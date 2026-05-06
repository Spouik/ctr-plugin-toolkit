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

#include "text_box.h"

extern "C" {
#include <3DS/util/utf.h>
}

#include <cstdarg>
#include <string>

#include "renderer.h"

void TextBox::SetText(const char* format, ...) {
  char formatted[64];
  va_list args;

  va_start(args, format);
  size_t len = vsnprintf(formatted, sizeof(formatted), format, args);
  va_end(args);

  utf8_to_utf16(buffer_, (const u8*)formatted, len);

  if (font_ != nullptr) {
    height_ = font_->GetHeight();
    width_ = font_->GetWidth() * len;
  }
}

void TextBox::Update(Renderer& renderer) {
  if (font_ == nullptr) return;

  UpdateInternalState();

  const u16* glyph = buffer_;
  const u16 width = font_->GetWidth();
  const u16 height = font_->GetHeight();
  
  s16 x = x_;
  while (*glyph != 0) {
    const u8* alpha = font_->GetGlyphTexture(*glyph);
    renderer.DrawGlyph(x, y_, width, height, color_, alpha);
    x += width;
    glyph++;
  }
}