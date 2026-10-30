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

#ifndef CTR_PLUGIN_TOOLKIT_TEST_APPLICATION_H
#define CTR_PLUGIN_TOOLKIT_TEST_APPLICATION_H
#include "background.h"
#include "entity.h"
#include "picture.h"
#include "text_box.h"
#include "config_file.h"


class HomeApplication : public Application {
public:
    HomeApplication();
    ~HomeApplication() override = default;
    void Update(Renderer& renderer) override;
    void OnClosing(CTRPluginFramework::Config& config);

private:
    void LoadTheme() const;

    bool debug_ = false;
    bool delock_ = false;
    bool old_odds_ = false;
    bool is_dark_theme_ = false;
    u16 title_color_ = 0x0000;

    Background& top_background_;
    Background& bottom_background_;

    //   Picture button_notebook_;
    //   Picture button_medallium_;
    //   Picture button_settings_;

    //   Picture icon_coin_;
    //   Picture icon_soul_;

    Picture debug_option_;
    Picture debug_toggle_;
    TextBox debug_text_;

    Picture delock_option_;
    Picture delock_toggle_;
    TextBox delock_text_;

    Picture old_odds_option_;
    Picture old_odds_toggle_;
    TextBox old_odds_text_;

    Picture back_pattern_top_;
    Picture back_pattern_bottom_;
    TextBox title_;
    Font mono_font_;
};

#endif // CTR_PLUGIN_TOOLKIT_TEST_APPLICATION_H