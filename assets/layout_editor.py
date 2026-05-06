from PIL import Image
from pathlib import Path

LAYOUTS = {
    "home": [
        ("medallium", 12, 39),
        ("notebook", 85, 39),
        ("settings", 158, 39),
    ],
    "status": [
        ("soul", 10, 10),
        ("coin", 50, 10),
    ]
}

BASE_DIR = Path.cwd() / "layout"
BACKGROUND_DIR = Path.cwd() / "background"
OUTPUT_BIN = Path.cwd() / "romfs.bin"
OUTPUT_HEADER = Path.cwd().parent / "include" / "romfs.h"

BASE_DIR.mkdir(parents=True, exist_ok=True)
BACKGROUND_DIR.mkdir(parents=True, exist_ok=True)
OUTPUT_HEADER.parent.mkdir(parents=True, exist_ok=True)

def RGB888_TO_RGB565(r, g, b):
    return ((b >> 3) & 0x1F) | (((g >> 2) & 0x3F) << 5) | (((r >> 3) & 0x1F) << 11)

class Background:
    def __init__(self, filepath):
        self.name = filepath.stem
        self.img = Image.open(filepath).convert("RGB")
        self.w, self.h = self.img.size
        self.pixels = self.img.load()

    def dump_gfx_bytes(self):
        buf = bytearray()
        for x in range(self.w):
            for y in range(self.h):
                y_inv = self.h - 1 - y
                r, g, b = self.pixels[x, y_inv]
                buf += RGB888_TO_RGB565(r, g, b).to_bytes(2, "little")
        return bytes(buf)

class Picture:
    def __init__(self, asset_dir, filename, x, y):
        self.x = x
        self.y = y
        self.img = Image.open(asset_dir / f"{filename}.png").convert("RGBA")
        self.w, self.h = self.img.size
        self.pixels = self.img.load()

    def dump_gfx_bytes(self):
        buf = bytearray()
        buf += self.x.to_bytes(2, "little")
        buf += self.y.to_bytes(2, "little")
        buf += self.w.to_bytes(2, "little")
        buf += self.h.to_bytes(2, "little")

        for x in range(self.w):
            for y in range(self.h):
                y_inv = self.h - 1 - y
                r, g, b, _ = self.pixels[x, y_inv]
                buf += RGB888_TO_RGB565(r, g, b).to_bytes(2, "little")

        for x in range(self.w):
            for y in range(self.h):
                y_inv = self.h - 1 - y
                buf.append(self.pixels[x, y_inv][3])
        return bytes(buf)

def build_romfs():
    layout_offsets = {}
    layout_assets = {}
    background_offsets = {}
    current_offset = 0

    with open(OUTPUT_BIN, "wb") as romfs:
        background_files = sorted(BACKGROUND_DIR.glob("*.png"))
        for filepath in background_files:
            bg = Background(filepath)
            data = bg.dump_gfx_bytes()
            background_offsets[bg.name] = current_offset
            romfs.write(data)
            current_offset += len(data)

        for layout_name, pictures in LAYOUTS.items():
            layout_offsets[layout_name] = current_offset
            layout_assets[layout_name] = {}
            asset_dir = BASE_DIR / layout_name
            asset_dir.mkdir(parents=True, exist_ok=True)
            preview = Image.new("RGBA", (320, 240), (0, 0, 0, 0))

            for pic_name, x, y in pictures:
                pic = Picture(asset_dir, pic_name, x, y)
                data = pic.dump_gfx_bytes()
                romfs.write(data)
                preview.paste(pic.img, (x, y), pic.img)
                layout_assets[layout_name][pic_name] = current_offset - layout_offsets[layout_name]
                current_offset += len(data)
            preview.save(BASE_DIR / f"preview-{layout_name}.png")

    lines = [
        "#ifndef CTR_PLUGIN_TOOLKIT_ROMFS_H",
        "#define CTR_PLUGIN_TOOLKIT_ROMFS_H",
        "",
        f'#define ROMFS_FILENAME "{OUTPUT_BIN.name}"',
        "",
        "// Backgrounds"
    ]

    for name, offset in background_offsets.items():
        lines.append(f"#define ROMFS_BACKGROUND_{name.upper()} 0x{offset:08X}")

    lines.append("\n// Layouts")
    for name, offset in layout_offsets.items():
        lines.append(f"#define ROMFS_LAYOUT_{name.upper()} 0x{offset:08X}")

    lines.append("")
    for layout_name, assets in layout_assets.items():
        for pic_name, rel_offset in assets.items():
            lines.append(
                f"#define ROMFS_LAYOUT_{layout_name.upper()}_{pic_name.upper()} "
                f"(ROMFS_LAYOUT_{layout_name.upper()}+0x{rel_offset:08X})"
            )

    lines.append("\n#endif")
    OUTPUT_HEADER.write_text("\n".join(lines))

if __name__ == "__main__":
    build_romfs()