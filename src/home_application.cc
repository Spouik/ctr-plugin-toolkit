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

#include "application_manager.h"
#include "home_application.h"

#include "background.h"
#include "renderer.h"
#include "romfs.h"

HomeApplication::HomeApplication()
    : top_background_(Background::GetTop()),
    bottom_background_(Background::GetBottom()),
    // button_notebook_(ROMFS_LAYOUT_HOME_NOTEBOOK),
    // button_medallium_(ROMFS_LAYOUT_HOME_MEDALLIUM),
    // button_settings_(ROMFS_LAYOUT_HOME_SETTINGS),
    // icon_coin_(ROMFS_LAYOUT_STATUS_COIN),
    // icon_soul_(ROMFS_LAYOUT_STATUS_SOUL),
    debug_option_(ROMFS_LAYOUT_TEXT_BOX_TEXT_BOX_BOTTOM),
    debug_toggle_(ROMFS_LAYOUT_TEXT_BOX_TOGGLE_OFF_BOTTOM),
    delock_option_(ROMFS_LAYOUT_TEXT_BOX_TEXT_BOX_BOTTOM),
    delock_toggle_(ROMFS_LAYOUT_TEXT_BOX_TOGGLE_OFF_BOTTOM),
    old_odds_option_(ROMFS_LAYOUT_TEXT_BOX_TEXT_BOX_BOTTOM),
    old_odds_toggle_(ROMFS_LAYOUT_TEXT_BOX_TOGGLE_OFF_BOTTOM),
    back_pattern_top_(ROMFS_LAYOUT_OTHERS_TOP_SCREEN),
    back_pattern_bottom_(ROMFS_LAYOUT_OTHERS_BOTTOM_SCREEN),
    mono_font_(ROMFS_FONT_SOURCE_CODE_PRO) {
    LoadTheme();
    std::string content = CTRPluginFramework::ConfigFile::getContentFromPath("config.ini");
    debug_ = CTRPluginFramework::ConfigFile::readParamBool("debug", content);
    delock_ = CTRPluginFramework::ConfigFile::readParamBool("delock", content);
    old_odds_ = CTRPluginFramework::ConfigFile::readParamBool("old_odds", content);
    // title_.SetFont(&mono_font_);
    // title_.SetText("ctr-plugin-toolkit v%d.%d", 1, 0);
    // title_.SetColor(title_color_);
    // title_.SetXY(25, 200);
    if (debug_)
    {
        debug_toggle_.SetTexture(ROMFS_LAYOUT_TEXT_BOX_TOGGLE_OFF_BOTTOM);
    }
    if (delock_)
    {
        delock_toggle_.SetTexture(ROMFS_LAYOUT_TEXT_BOX_TOGGLE_OFF_BOTTOM);
        delock_toggle_.SetXY(debug_toggle_.GetX(), debug_toggle_.GetY() + 50);
    }
    if (old_odds_)
    {
        old_odds_toggle_.SetTexture(ROMFS_LAYOUT_TEXT_BOX_TOGGLE_OFF_BOTTOM);
        old_odds_toggle_.SetXY(delock_toggle_.GetX(), delock_toggle_.GetY() + 50);
    }



    top_background_.SetIsAnimated(true);
    bottom_background_.SetIsAnimated(true);

    // button_settings_.SetOnRelease([this]() {
    //     is_dark_theme_ = !is_dark_theme_;
    //     LoadTheme();
    //     });
    // button_notebook_.SetOnRelease([this]() {
    //     top_background_.ToggleIsHorizontal();
    //     bottom_background_.ToggleIsHorizontal();
    //     });
    // button_medallium_.SetOnRelease([this]() {
    //     top_background_.ToggleDelta();
    //     bottom_background_.ToggleDelta();
    //     });

    // DEBUG
    debug_toggle_.SetOnRelease([this]() {
        if (debug_)
        {
            debug_toggle_.SetTexture(ROMFS_LAYOUT_TEXT_BOX_TOGGLE_OFF_BOTTOM);
        }
        else
        {
            debug_toggle_.SetTexture(ROMFS_LAYOUT_TEXT_BOX_TOGGLE_ON_BOTTOM);
        }

        debug_ = !debug_;
        });

    debug_text_.SetFont(&mono_font_);
    debug_text_.SetText("Mode Debug\n");
    debug_text_.SetColor(title_color_);
    debug_text_.SetXY(25, 22);


    // DELOCK
    delock_toggle_.SetOnRelease([this]() {
        if (delock_)
        {
            delock_toggle_.SetTexture(ROMFS_LAYOUT_TEXT_BOX_TOGGLE_OFF_BOTTOM);
            delock_toggle_.SetXY(debug_toggle_.GetX(), debug_toggle_.GetY() + 50);
        }
        else
        {
            delock_toggle_.SetTexture(ROMFS_LAYOUT_TEXT_BOX_TOGGLE_ON_BOTTOM);
            delock_toggle_.SetXY(debug_toggle_.GetX(), debug_toggle_.GetY() + 50);
        }

        delock_ = !delock_;
        });

    delock_option_.SetXY(0, debug_option_.GetY() + 50);
    delock_toggle_.SetXY(debug_toggle_.GetX(), debug_toggle_.GetY() + 50);
    delock_text_.SetFont(&mono_font_);
    delock_text_.SetText("Enlever le Shiny Lock\n");
    delock_text_.SetColor(title_color_);
    delock_text_.SetXY(25, debug_text_.GetY() + 50);

    // DELOCK
    old_odds_toggle_.SetOnRelease([this]() {
        if (old_odds_)
        {
            old_odds_toggle_.SetTexture(ROMFS_LAYOUT_TEXT_BOX_TOGGLE_OFF_BOTTOM);
            old_odds_toggle_.SetXY(delock_toggle_.GetX(), delock_toggle_.GetY() + 50);
        }
        else
        {
            old_odds_toggle_.SetTexture(ROMFS_LAYOUT_TEXT_BOX_TOGGLE_ON_BOTTOM);
            old_odds_toggle_.SetXY(delock_toggle_.GetX(), delock_toggle_.GetY() + 50);
        }

        old_odds_ = !old_odds_;
        });

    old_odds_option_.SetXY(0, delock_option_.GetY() + 50);
    old_odds_toggle_.SetXY(delock_toggle_.GetX(), delock_toggle_.GetY() + 50);
    old_odds_text_.SetFont(&mono_font_);
    old_odds_text_.SetText("Taux 1/8192\n");
    old_odds_text_.SetColor(title_color_);
    old_odds_text_.SetXY(25, delock_text_.GetY() + 50);


}

void HomeApplication::Update(Renderer& renderer) {
    const u16 kBackgroundColor = is_dark_theme_ ? 0x0000 : 0xffff;

    renderer.SetTargetScreen(kTopScreen);
    // top_background_.Update(renderer);
    // back_pattern_top_.Update(renderer);

    // icon_coin_.Update(renderer);
    // icon_soul_.Update(renderer);

    renderer.SetTargetScreen(kBottomScreen);
    bottom_background_.Update(renderer);
    back_pattern_bottom_.Update(renderer);
    debug_option_.Update(renderer);
    debug_toggle_.Update(renderer);
    debug_text_.Update(renderer);
    delock_option_.Update(renderer);
    delock_toggle_.Update(renderer);
    delock_text_.Update(renderer);
    old_odds_option_.Update(renderer);
    old_odds_toggle_.Update(renderer);
    old_odds_text_.Update(renderer);
    // button_notebook_.Update(renderer);
    // button_medallium_.Update(renderer);
    // button_settings_.Update(renderer);
    // title_.Update(renderer);
}

void HomeApplication::LoadTheme() const {
    top_background_.SetTexture(ROMFS_BACKGROUND_TOP_SCREEN);
    bottom_background_.SetTexture(ROMFS_BACKGROUND_BOTTOM_SCREEN);
    // if (is_dark_theme_) {
    //     bottom_background_.SetTexture(ROMFS_BACKGROUND_BOTTOM_DARK);
    // }
    // else {
    //     bottom_background_.SetTexture(ROMFS_BACKGROUND_BOTTOM_LIGHT);
    // }


}

void HomeApplication::OnClosing(CTRPluginFramework::Config& config) {
    config.debug = debug_;
    config.delock = delock_;
    config.old_odds = old_odds_;
    CTRPluginFramework::ConfigFile::write(config, "config.ini");
    if (config.delock)
    {
        CTRPluginFramework::Process::Write8(0x0014F690, 0x09);
    }
    else
    {
        CTRPluginFramework::Process::Write8(0x0014F690, 0x18);
    }

    if (config.old_odds)
    {
        CTRPluginFramework::Process::Write8(0x00168adc, 0x08);
    }
    else
    {
        CTRPluginFramework::Process::Write8(0x00168adc, 0x10);
    }

}