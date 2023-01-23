#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

class GuiSlider : public GuiControl
{
public:
    GuiSlider(uint32 id, SDL_Rect bounds);
    virtual ~GuiSlider();

    bool Update(float dt);
    bool Draw(Render* render);

    void SetValue(int value);
    int GetValue();

    SDL_Texture* knobTex;
    SDL_Texture* barTex;

    int value;
    int knobX;

private:
    int minValue;
    int maxValue;

    int mouseX, mouseY;
    unsigned int click;
};

#endif // __GUISLIDER_H__
