#include <CTRPluginFramework/System/File.hpp>

#include "picture_text.h"
#include "renderer.h"
#include "romfs.h"

using namespace CTRPluginFramework;

PictureText::PictureText() : Picture() {
    _text = TextBox();
}

PictureText::PictureText(u32 offset, s16 xOffset, s16 yOffset) : Picture(offset) {
    _text = TextBox();
    _text.SetXY(GetX() + xOffset, GetY() + yOffset);
}

PictureText::PictureText(u32 offset, s16 xOffset, s16 yOffset, Font* font, const char* format, ...) : Picture(offset) {
    _text = TextBox();
    _text.SetXY(GetX() + xOffset, GetY() + yOffset);
    _text.SetFont(font);
    _text.SetText(format);
}

void PictureText::Update(Renderer& renderer) {
    this->Update(renderer);
    _text.Update(renderer);
}

TextBox PictureText::getText() const {
    return _text;
}