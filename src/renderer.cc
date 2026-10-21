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

void Renderer::DrawRect(s16 x, s16 y, u16 width, u16 height, u16 color) const {
    u16* fb = GetFrameBuffer(x, y + height - 1);
    while (width--) {
        s16 h = height;
        while (h--) {
            *fb++ = color;
        }
        fb += kScreenHeight - height;
    }
}

void Renderer::DrawPicture(s16 x, s16 y, u16 width, u16 height,
    const u16* color,
    const u8* alpha) const {
    u16* fb = GetFrameBuffer(x, y + height - 1);

    if (alpha == nullptr) {
        while (width--) {
            s32 h = height;
            while (h--) {
                *fb++ = *color++;
            }
            fb += kScreenHeight - height;
        }
        return;
    }

    while (width--) {
        s32 h = height;
        while (h--) {
            u8 a = *alpha++;

            if (a == 255) {
                *fb++ = *color++;
            }
            else if (a == 0) {
                fb++;
                color++;
            }
            else {
                u16 oc = *fb;
                u16 nc = *color++;

                u32 weight_n = a >> 2; // 0-63
                u32 weight_o = 64 - weight_n; // 64-1

                u32 rb = ((nc & 0xF81F) * weight_n + (oc & 0xF81F) * weight_o) >> 6;
                u32 g = ((nc & 0x07E0) * weight_n + (oc & 0x07E0) * weight_o) >> 6;

                *fb++ = (u16)((rb & 0xF81F) | (g & 0x07E0));
            }
        }
        fb += kScreenHeight - height;
    }
}

void Renderer::DrawBackground(u16 width, u16 height,
    const u16* color) const {
    u16* fb = GetFrameBuffer(0, height - 1);
    const u16* max = GetFrameBuffer(width, 0);
    while (fb < max) {
        *fb++ = *color++;
    }
}

void Renderer::DrawAnimatedBackground(u16 width, u16 height,
    const u16* color,
    bool is_horizontal,
    s32 offset) const {
    if (is_horizontal) {
        offset %= width;
        if (offset < 0) offset += width;

        const u16* max = GetFrameBuffer(width, 0);

        u16* fb = GetFrameBuffer(offset, height - 1);
        const u16* src = color;

        int w = width - offset;
        while (w--) {
            int h = height;
            while (h--) {
                if (fb <= max)
                    *fb++ = *src++;
            }
            fb += kScreenHeight - height;
        }

        fb = GetFrameBuffer(0, height - 1);

        w = offset;
        while (w--) {
            int h = height;
            while (h--) {
                if (fb <= max)
                    *fb++ = *src++;
            }
            fb += kScreenHeight - height;
        }
    }
    else {
        offset %= height;
        if (offset < 0) offset += height;

        for (int col = 0; col < (int)width; col++) {
            const u16* src_col = color + col * height;

            u16* fb = GetFrameBuffer(col, height - 1);

            const u16* s = src_col + offset;
            int h = height - offset;
            while (h--) {
                *fb++ = *s++;
            }

            s = src_col;
            h = offset;
            while (h--) {
                *fb++ = *s++;
            }
        }
    }
}

void Renderer::DrawGlyph(s16 x, s16 y, u16 width, u16 height,
    u16 color,
    const u8* alpha) const {
    u16* fb = GetFrameBuffer(x, y + height - 1);

    while (width--) {
        s32 h = height;
        while (h--) {
            u8 a = *alpha++;

            if (a == 0) {
                fb++;
            }
            else if (a == 255) {
                *fb++ = color;
            }
            else {
                u16 oc = *fb;
                u16 nc = color;

                u32 weight_n = a >> 2; // 0-63
                u32 weight_o = 64 - weight_n; // 64-1

                u32 rb = ((nc & 0xF81F) * weight_n + (oc & 0xF81F) * weight_o) >> 6;
                u32 g = ((nc & 0x07E0) * weight_n + (oc & 0x07E0) * weight_o) >> 6;

                *fb++ = (u16)((rb & 0xF81F) | (g & 0x07E0));
            }
        }
        fb += kScreenHeight - height;
    }
}