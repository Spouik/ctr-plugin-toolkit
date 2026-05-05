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

#include "picture.h"

#include "renderer.h"

#include <CTRPluginFramework/System/File.hpp>

#include "romfs.h"

using namespace CTRPluginFramework;

struct Header {
  s16 x;
  s16 y;
  u16 width;
  u16 height;
};

Picture::Picture(u32 offset) : Pane() {
  Header header;
  File file;

  File::Open(file, ROMFS_FILENAME, File::READ);

  file.Seek(offset, File::SET);
  file.Read(&header, sizeof(header));
  x_ = header.x;
  y_ = header.y;
  width_ = header.width;
  height_ = header.height;

  color_buffer_ = std::unique_ptr<u16[]>(new u16[width_ * height_]);
  file.Read(color_buffer_.get(), width_ * height_ * sizeof(u16));

  alpha_buffer_ = std::unique_ptr<u8[]>(new u8[width_ * height_]);
  file.Read(alpha_buffer_.get(), width_ * height_ * sizeof(u8));

  file.Flush();
  file.Close();
}

void Picture::Update(Renderer& renderer) {
  UpdateInternalState();
  renderer.DrawPicture(x_, y_, width_, height_, color_buffer_.get(),
                       alpha_buffer_.get());
}