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

#include "romfs.h"
#include "font.h"

#include <CTRPluginFramework/System/File.hpp>

using namespace CTRPluginFramework;

Font::Font(u32 offset) {
  File file;

  File::Open(file, ROMFS_FILENAME, File::READ);
  file.Seek(offset, File::SET);
  file.Read(&glyph_, sizeof(glyph_));

  code_ = std::unique_ptr<u16[]>(new u16[glyph_.count]);
  file.Read(code_.get(), glyph_.count * sizeof(u16));

  const u32 size = glyph_.count * glyph_.width * glyph_.height;
  alpha_buffer_ = std::unique_ptr<u8[]>(new u8[size]);
  file.Read(alpha_buffer_.get(), size);

  offset_ = std::unique_ptr<u8[]>(new u8[glyph_.count]);
  file.Read(offset_.get(), glyph_.count);

  file.Flush();
  file.Close();
}