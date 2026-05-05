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

#include "test_application.h"

#include "renderer.h"

TestApplication::TestApplication() : btn_switch_theme_(10, 10, 50, 50, 0x001f) {
  btn_switch_theme_.SetOnRelease([this]() {
    is_dark_theme_ ^= true;
  });
}

void TestApplication::Update(Renderer& renderer) {
  const u16 kColor = is_dark_theme_ ? 0x0000 : 0xffff;

  renderer.SetTargetScreen(kTopScreen);
  renderer.DrawRect(0, 0, 400, 240, kColor);

  renderer.SetTargetScreen(kBottomScreen);
  renderer.DrawRect(0, 0, 320, 240, kColor);
  btn_switch_theme_.Update(renderer);
}