#ifndef CTR_PLUGIN_TOOLKIT_PICTURE_TEXT_H
#define CTR_PLUGIN_TOOLKIT_PICTURE_TEXT_H

#include "picture.h"
#include "text_box.h"

class TextBox;
class PictureText : public Picture {
    private: 
    TextBox _text;
    public:
    PictureText();
    ~PictureText() override = default;
    PictureText(u32 offset, s16 xOffset, s16 yOffset);
    PictureText(u32 offset,
                s16 xOffset,
                s16 yOffset,
                Font* font,
                const char* format, ...);
    
    void Update(Renderer& renderer) override;
    TextBox getText() const;
};



#endif // CTR_PLUGIN_TOOLKIT_PICTURE_TEXT_H