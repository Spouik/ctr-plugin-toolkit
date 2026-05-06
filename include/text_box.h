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

#ifndef CTR_PLUGIN_TOOLKIT_TEXT_BOX_H
#define CTR_PLUGIN_TOOLKIT_TEXT_BOX_H
#include <string>

#include "font.h"
#include "pane.h"

class TextBox : public Pane {
public:
  TextBox() = default;
  ~TextBox() override = default;

  void SetFont(Font* font) { font_ = font; }
  void SetText(const char* format, ...);
  void Update(Renderer& renderer) override;

private:
  u16 buffer_[64] = {};
  Font* font_ = nullptr;
};

#endif //CTR_PLUGIN_TOOLKIT_TEXT_BOX_H