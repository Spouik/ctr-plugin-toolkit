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

#include "renderer.h"
#include "pane.h"

#include <CTRPluginFramework/System/Touch.hpp>

using namespace CTRPluginFramework;

void Pane::Update(Renderer& renderer) {
  UpdateInternalState();
  renderer.DrawRect(x_, y_, width_, height_, color_);
}

void Pane::UpdateInternalState() {
  const bool is_down = Touch::IsDown();
  switch (state_) {
    case State::kIdle: {
      const UIntVector pos = Touch::GetPosition();
      const bool contains =
          pos.x >= x_ && pos.x <= x_ + width_ &&
          pos.y >= y_ && pos.y <= y_ + height_;
      if (is_down && contains) state_ = State::kHeld;
    }
    break;
    case State::kHeld:
      if (!is_down) state_ = State::kReleased;
      break;
    case State::kReleased:
      if (on_release_ != nullptr) on_release_();
      state_ = State::kIdle;
      break;
  }
}