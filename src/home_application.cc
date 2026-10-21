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

#include "home_application.h"

#include "background.h"
#include "renderer.h"
#include "romfs.h"

HomeApplication::HomeApplication()
    : top_background_(Background::GetTop()),
    bottom_background_(Background::GetBottom()),
    button_notebook_(ROMFS_LAYOUT_HOME_NOTEBOOK),
    button_medallium_(ROMFS_LAYOUT_HOME_MEDALLIUM),
    button_settings_(ROMFS_LAYOUT_HOME_SETTINGS),
    icon_coin_(ROMFS_LAYOUT_STATUS_COIN),
    icon_soul_(ROMFS_LAYOUT_STATUS_SOUL),
    mono_font_(ROMFS_FONT_SOURCE_CODE_PRO) {
    LoadTheme();

    title_.SetFont(&mono_font_);
    title_.SetText("ctr-plugin-toolkit v%d.%d", 1, 0);
    title_.SetColor(title_color_);
    title_.SetXY(10, 200);

    top_background_.SetIsAnimated(true);
    bottom_background_.SetIsAnimated(true);

    button_settings_.SetOnRelease([this]() {
        is_dark_theme_ = !is_dark_theme_;
        LoadTheme();
        });
    button_notebook_.SetOnRelease([this]() {
        top_background_.ToggleIsHorizontal();
        bottom_background_.ToggleIsHorizontal();
        });
    button_medallium_.SetOnRelease([this]() {
        top_background_.ToggleDelta();
        bottom_background_.ToggleDelta();
        });

    title_.SetOnRelease([this]() {
        title_color_ ^= 0xffff;
        title_.SetColor(title_color_);
        });
}

void HomeApplication::Update(Renderer& renderer) {
    const u16 kBackgroundColor = is_dark_theme_ ? 0x0000 : 0xffff;

    renderer.SetTargetScreen(kTopScreen);
    top_background_.Update(renderer);
    icon_coin_.Update(renderer);
    icon_soul_.Update(renderer);

    renderer.SetTargetScreen(kBottomScreen);
    bottom_background_.Update(renderer);
    button_notebook_.Update(renderer);
    button_medallium_.Update(renderer);
    button_settings_.Update(renderer);
    title_.Update(renderer);
}

void HomeApplication::LoadTheme() const {
    top_background_.SetTexture(ROMFS_BACKGROUND_TOP_SCREEN);
    if (is_dark_theme_) {
        bottom_background_.SetTexture(ROMFS_BACKGROUND_BOTTOM_DARK);
    }
    else {
        bottom_background_.SetTexture(ROMFS_BACKGROUND_BOTTOM_LIGHT);
    }
}