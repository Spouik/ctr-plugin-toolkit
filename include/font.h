/*
 * MIT License
 *
 * Copyright (c) 2026 David Darras
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef CTR_PLUGIN_TOOLKIT_FONT_H
#define CTR_PLUGIN_TOOLKIT_FONT_H
#include <types.h>

#include <memory>

class Font {
public:
  Font(u32 offset);

  u16 GetWidth() const { return glyph_.width; };
  u16 GetHeight() const { return glyph_.height; };

  u32 GetIndex(u16 code) const {
    for (u32 i = 0; i < glyph_.count; i++) {
      if (code_.get()[i] == code) {
        return i;
      }
    }
    return 0;
  }

  u8* GetGlyphTexture(u16 code) const {
    u32 index = GetIndex(code);
    return alpha_buffer_.get() + index * glyph_.width * glyph_.height;
  };

protected:
  struct Glyph {
    u16 magic;
    u16 count;
    u16 width;
    u16 height;
  };

  Glyph glyph_;
  std::unique_ptr<u16[]> code_;
  std::unique_ptr<u8[]> alpha_buffer_;
  std::unique_ptr<u8[]> offset_;
};

#endif // CTR_PLUGIN_TOOLKIT_FONT_H