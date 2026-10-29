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

#ifndef CTR_PLUGIN_TOOLKIT_PICTURE_H
#define CTR_PLUGIN_TOOLKIT_PICTURE_H
#include <memory>

#include "pane.h"

class Picture : public Pane {
public:
    Picture() = default;
    ~Picture() override = default;
    Picture(u32 offset);

    void SetTexture(u32 offset);
    void Update(Renderer& renderer) override;

protected:
    struct Header {
        s16 x;
        s16 y;
        u16 width;
        u16 height;
    };

    std::unique_ptr<u16[]> color_buffer_ = nullptr;
    std::unique_ptr<u8[]> alpha_buffer_ = nullptr;
};

#endif //CTR_PLUGIN_TOOLKIT_PICTURE_H