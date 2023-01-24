#include "GuiSlider.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"
#include "Window.h"
#include "Textures.h"


GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds) : GuiControl(GuiControlType::SLIDER, id)
{
    this->bounds = bounds;
    this->minValue = bounds.x / app->win->GetScale() + 8;
    this->maxValue = (bounds.x + bounds.w) / app->win->GetScale() + 8;
    barTex = app->tex->Load("Assets/Textures/Slider.png");
    knobTex = app->tex->Load("Assets/Textures/Knob.png");

    SetValue((bounds.w - 8) / 2);
}

GuiSlider::~GuiSlider()
{
    delete knobTex;
}

bool GuiSlider::Update(float dt)
{
    if (state != GuiControlState::DISABLED && state != GuiControlState::OFF)
    {
        app->input->GetMousePosition(mouseX, mouseY);

        GuiControlState previousState = state;

        if (mouseX * app->win->GetScale() >= bounds.x && mouseX * app->win->GetScale() <= bounds.x + bounds.w * app->win->GetScale() &&
            mouseY * app->win->GetScale() >= bounds.y && mouseY * app->win->GetScale() <= bounds.y + bounds.h * app->win->GetScale())
        {
            state = GuiControlState::FOCUSED;

            if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
            {
                state = GuiControlState::PRESSED;
                knobX = mouseX - bounds.x / app->win->GetScale() - 4;
                if (knobX < 0) knobX = 0;
                if (knobX > bounds.w - 8) knobX = bounds.w - 8;
                NotifyObserver();
            }
        }
        else {
            if (state == GuiControlState::PRESSED && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) != KeyState::KEY_UP) {
                knobX = mouseX - bounds.x / app->win->GetScale() - 4;
                if (knobX < 0) knobX = 0;
                if (knobX > bounds.w - 8) knobX = bounds.w - 8;
                NotifyObserver();
                
                
            }
            else {
                state = GuiControlState::NORMAL;
            }
        }
       
        value = knobX / float(bounds.w - 8) * 100;
        
        

    }

    return false;
}


bool GuiSlider::Draw(Render* render)
{
    // Draw the slider
    if (state != GuiControlState::OFF)
    {
        render->DrawTexture(barTex, bounds.x / app->win->GetScale(), bounds.y / app->win->GetScale());
        render->DrawTexture(knobTex, bounds.x / app->win->GetScale() + knobX, bounds.y / app->win->GetScale(), new SDL_Rect{ 0,0,8,8 });
    }
    return true;
}

void GuiSlider::SetValue(int value)
{
    this->value = value;
    knobX = value * float(bounds.w - 8) / 100;
}

int GuiSlider::GetValue()
{
    return value; 
}
