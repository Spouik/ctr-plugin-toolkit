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

#ifndef CTR_PLUGIN_TOOLKIT_BACKGROUND_H
#define CTR_PLUGIN_TOOLKIT_BACKGROUND_H

#include "picture.h"

class Background : public Picture {
public:
  ~Background() override = default;

  Background(const Background&) = delete;
  Background& operator=(const Background&) = delete;
  Background(Background&&) = delete;
  Background& operator=(Background&&) = delete;

  static Background& GetTop() {
    static Background instance(400, 240);
    return instance;
  }

  static Background& GetBottom() {
    static Background instance(320, 240);
    return instance;
  }

  void Update(Renderer& renderer) override;

  void SetTexture(u32 offset);
  void SetIsAnimated(bool is_animated) { is_animated_ = is_animated; }
  bool IsAnimated() const { return is_animated_; }
  s32 GetDelta() const { return delta_; }
  void SetDelta(s32 delta) { delta_ = delta; }
  void ToggleDelta() { delta_ = -delta_; }
  void ToggleIsHorizontal() { is_horizontal_ = !is_horizontal_; }
  bool IsHorizontal() const { return is_horizontal_; }
  void SetIsHorizontal(bool is_horizontal) { is_horizontal_ = is_horizontal; }

private:
  Background() = default;
  Background(u32 width, u32 height);

  // Parameters for the animation
  s32 offset_ = 0;
  s32 delta_ = 1;
  bool is_horizontal_ = true;
  bool is_animated_ = false;
};

#endif // CTR_PLUGIN_TOOLKIT_BACKGROUND_H