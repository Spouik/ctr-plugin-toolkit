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

#ifndef CTR_PLUGIN_TOOLKIT_RENDERER_H
#define CTR_PLUGIN_TOOLKIT_RENDERER_H

#include <CTRPluginFramework/Graphics/OSD.hpp>

enum TargetScreen {
  kTopScreen = 0,
  kBottomScreen = 1
};

class Renderer {
public:
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;
  Renderer(Renderer&&) = delete;
  Renderer& operator=(Renderer&&) = delete;

  static Renderer& GetInstance() {
    static Renderer instance;
    return instance;
  }

  void SetTargetScreen(const TargetScreen target_screen) {
    target_screen_ = target_screen;
  }

  u16* GetFrameBuffer(const s32 x, const s32 y) const {
    u16* base = target_screen_ == kTopScreen
                  ? (u16*)top_screen_.LeftFramebuffer
                  : (u16*)bottom_screen_.LeftFramebuffer;
    const u32 offset = kScreenHeight - 1 - y + x * kScreenHeight;
    return base + offset;
  }

  void DrawRect(s16 x, s16 y, u16 width, u16 height, u16 color) const;

  void DrawPicture(s16 x, s16 y, u16 width, u16 height,
                   const u16* color,
                   const u8* alpha) const;

  void DrawGlyph(s16 x, s16 y, u16 width, u16 height,
                 u16 color,
                 const u8* alpha) const;

  void DrawBackground(u16 width, u16 height,
                      const u16* color) const;

  void DrawAnimatedBackground(u16 width, u16 height,
                              const u16* color,
                              bool is_horizontal,
                              s32 offset) const;

private:
  Renderer() = default;

  static constexpr u32 kTopScreenWidth = 400;
  static constexpr u32 kBottomScreenWidth = 320;
  static constexpr u32 kScreenHeight = 240;

  const CTRPluginFramework::Screen& top_screen_ =
      CTRPluginFramework::OSD::GetTopScreen();
  const CTRPluginFramework::Screen& bottom_screen_ =
      CTRPluginFramework::OSD::GetBottomScreen();

  TargetScreen target_screen_ = kTopScreen;
};

#endif // CTR_PLUGIN_TOOLKIT_RENDERER_H