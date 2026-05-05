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
BASE_DIR.mkdir(parents=True, exist_ok=True)

OUTPUT_FILENAME = "romfs.bin"
OUTPUT_BIN = Path.cwd() / OUTPUT_FILENAME
OUTPUT_HEADER = Path.cwd().parent / "include" / "romfs.h"
OUTPUT_HEADER.parent.mkdir(parents=True, exist_ok=True)


def RGB888_TO_RGB565(r, g, b):
    return ((b >> 3) & 0x1F) | (((g >> 2) & 0x3F) << 5) | (((r >> 3) & 0x1F) << 11)


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
                y_inv = self.h - y - 1
                r, g, b, _ = self.pixels[x, y_inv]
                buf += RGB888_TO_RGB565(r, g, b).to_bytes(2, "little")

        for x in range(self.w):
            for y in range(self.h):
                y_inv = self.h - y - 1
                buf.append(self.pixels[x, y_inv][3])

        return bytes(buf)


def build_romfs():
    layout_offsets = {}
    layout_assets = {}
    current_offset = 0

    romfs = open(OUTPUT_BIN, "wb")

    for layout_name, pictures in LAYOUTS.items():
        layout_offsets[layout_name] = current_offset
        layout_assets[layout_name] = {}

        asset_dir = BASE_DIR / layout_name
        asset_dir.mkdir(parents=True, exist_ok=True)

        preview = Image.new("RGBA", (320, 240), (255, 255, 255, 255))

        for pic_name, x, y in pictures:
            pic = Picture(asset_dir, pic_name, x, y)
            data = pic.dump_gfx_bytes()

            romfs.write(data)
            preview.paste(pic.img, (x, y), pic.img)

            layout_assets[layout_name][pic_name] = current_offset - layout_offsets[layout_name]
            current_offset += len(data)

        preview.save(BASE_DIR / f"preview-{layout_name}.png")

    romfs.close()

    lines = []
    lines.append("#ifndef CTR_PLUGIN_TOOLKIT_ROMFS_H")
    lines.append("#define CTR_PLUGIN_TOOLKIT_ROMFS_H")
    lines.append("")

    lines.append(f"#define ROMFS_FILENAME \"{OUTPUT_FILENAME}\"")

    for name, offset in layout_offsets.items():
        lines.append(f"#define ROMFS_LAYOUT_{name.upper()} 0x{offset:08X}")

    lines.append("")

    for layout_name, assets in layout_assets.items():
        for pic_name, offset in assets.items():
            lines.append(
                f"#define ROMFS_LAYOUT_{layout_name.upper()}_{pic_name.upper()} "
                f"(ROMFS_LAYOUT_{layout_name.upper()}+0x{offset:08X})"
            )

    lines.append("")
    lines.append("#endif // CTR_PLUGIN_TOOLKIT_ROMFS_H")

    OUTPUT_HEADER.write_text("\n".join(lines))


build_romfs()
