import io
from PIL import Image
from pathlib import Path

LAYOUTS = {
    "others": [
        ("top_screen", 0, 0)
    ]
}

BASE_DIR = Path.cwd() / "layout"
BACKGROUND_DIR = Path.cwd() / "background"
FONT_DIR = Path.cwd() / "font"

OUTPUT_BIN = Path.cwd() / "romfs.bin"
OUTPUT_HEADER = Path.cwd().parent / "include" / "romfs.h"

BASE_DIR.mkdir(parents=True, exist_ok=True)
BACKGROUND_DIR.mkdir(parents=True, exist_ok=True)
FONT_DIR.mkdir(parents=True, exist_ok=True)
OUTPUT_HEADER.parent.mkdir(parents=True, exist_ok=True)

def RGB888_TO_RGB565(r, g, b):
    return ((b >> 3) & 0x1F) | (((g >> 2) & 0x3F) << 5) | (((r >> 3) & 0x1F) << 11)

def write(f, data, size):
    f.write(data.to_bytes(size, "little"))

def to_hex_pair(s):
    return int.from_bytes(s.encode("utf-16le"), "little")

def build_font_bytes(filepath):
    ROWS, COLS = 14, 16

    glyphs_str = """ !"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~▪€▪,ƒ„…†‡ˆ‰Š‹Œ▪Ž▪▪''""•–—˜™š›œ▪žŸ▪¡¢£¤¥¦§¨©ª«¬▪®¯°±²³´µ¶·▪¹º»¼½¾¿ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖ×ØÙÚÛÜÝÞßàáâãäåæçèéêëìíîïðñòóôõö÷øùúûüýþÿ"""

    glyph_bytes = glyphs_str.encode("utf-16le")
    glyphs = [glyph_bytes[i] + (glyph_bytes[i + 1] << 8)
              for i in range(0, len(glyph_bytes), 2)]

    img = Image.open(filepath).convert("RGBA")
    px = img.load()
    img_w, img_h = img.size

    glyph_w = img_w // COLS
    glyph_h = img_h // ROWS

    real_w = glyph_w - 4
    real_h = glyph_h - 6

    def extract(xi, yi):
        data = []
        base_x = 2 + xi * glyph_w
        base_y = 2 + yi * glyph_h

        for x in range(real_w):
            for y in range(real_h - 1, -1, -1):
                r = px[base_x + x, base_y + y][0]
                data.append(0xFF - r)
        return data

    glyph_bitmaps = [
        extract(x, y)
        for y in range(ROWS)
        for x in range(COLS)
    ]

    buf = io.BytesIO()

    write(buf, 0x3146, 2)
    write(buf, len(glyphs), 2)
    write(buf, real_w, 2)
    write(buf, real_h, 2)

    for g in glyphs:
        write(buf, g, 2)

    for bitmap in glyph_bitmaps:
        for val in bitmap:
            write(buf, val, 1)

    for _ in glyphs:
        write(buf, real_w, 1)

    return buf.getvalue()

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
    font_offsets = {}

    current_offset = 0

    with open(OUTPUT_BIN, "wb") as romfs:

        for filepath in sorted(BACKGROUND_DIR.glob("*.png")):
            bg = Background(filepath)
            data = bg.dump_gfx_bytes()
            background_offsets[bg.name] = current_offset
            romfs.write(data)
            current_offset += len(data)

        for layout_name, pictures in LAYOUTS.items():
            layout_offsets[layout_name] = current_offset
            layout_assets[layout_name] = {}

            asset_dir = BASE_DIR / layout_name
            preview = Image.new("RGBA", (320, 240), (0, 0, 0, 0))

            for pic_name, x, y in pictures:
                pic = Picture(asset_dir, pic_name, x, y)
                data = pic.dump_gfx_bytes()
                romfs.write(data)

                preview.paste(pic.img, (x, y), pic.img)
                layout_assets[layout_name][pic_name] = current_offset - layout_offsets[layout_name]

                current_offset += len(data)

            preview.save(BASE_DIR / f"preview-{layout_name}.png")

        for filepath in sorted(FONT_DIR.glob("*.bmp")):
            name = filepath.stem
            data = build_font_bytes(filepath)

            font_offsets[name] = current_offset
            romfs.write(data)
            current_offset += len(data)

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

    for layout_name, assets in layout_assets.items():
        for pic_name, rel_offset in assets.items():
            lines.append(
                f"#define ROMFS_LAYOUT_{layout_name.upper()}_{pic_name.upper()} "
                f"(ROMFS_LAYOUT_{layout_name.upper()}+0x{rel_offset:08X})"
            )

    lines.append("\n// Fonts")
    for name, offset in font_offsets.items():
        lines.append(f"#define ROMFS_FONT_{name.upper()} 0x{offset:08X}")

    lines.append("\n#endif")

    OUTPUT_HEADER.write_text("\n".join(lines))


if __name__ == "__main__":
    build_romfs()
