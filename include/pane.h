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

#ifndef CTR_PLUGIN_TOOLKIT_PANE_H
#define CTR_PLUGIN_TOOLKIT_PANE_H

#include <types.h>
#include <functional>
#include "entity.h"

class Pane : public Entity {
public:
  Pane() = default;

  Pane(const s16 x, const s16 y, const u16 width, const u16 height,
       const u16 color) :
    x_(x), y_(y), width_(width), height_(height), color_(color) {
  }

  ~Pane() override = default;

  void Update(Renderer& renderer) override;

  // Setters
  void SetX(const s16 x) { x_ = x; }
  void SetY(const s16 y) { y_ = y; }

  void SetXY(const s16 x, const s16 y) {
    x_ = x;
    y_ = y;
  }

  void SetWidth(const u16 width) { width_ = width; }
  void SetHeight(const u16 height) { height_ = height; }
  void SetColor(const u16 color) { color_ = color; }
  void SetIsVisible(const bool is_visible) { is_visible_ = is_visible; }

  void SetOnRelease(const std::function<void()>& on_release) {
    on_release_ = on_release;
  }

  // Getters
  s16 GetX() const { return x_; }
  s16 GetY() const { return y_; }
  u16 GetWidth() const { return width_; }
  u16 GetHeight() const { return height_; }
  u16 GetColor() const { return color_; }
  bool IsVisible() const { return is_visible_; }

protected:
  enum class State : u8 {
    kIdle,
    kHeld,
    kReleased
  };

  void UpdateInternalState();

  s16 x_ = 0;
  s16 y_ = 0;
  u16 width_ = 0;
  u16 height_ = 0;
  u16 color_ = 0xffff;
  bool is_visible_ = true;
  State state_ = State::kIdle;
  std::function<void()> on_release_ = nullptr;
};


#endif // CTR_PLUGIN_TOOLKIT_PANE_H