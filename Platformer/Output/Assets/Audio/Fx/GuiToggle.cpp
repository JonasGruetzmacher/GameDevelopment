#include "GuiToggle.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "Timer.h"
#include "Render.h"
#include "Log.h"

GuiToggle::GuiToggle(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::TOGGLE, id)
{
    this->bounds = bounds;
    this->text = text;

    canClick = true;
    drawBasic = false;

    //Load Button Click Sounds
    click1FxId = app->audio->LoadFx("Assets/Audio/Fx/click1.ogg");
    click2FxId = app->audio->LoadFx("Assets/Audio/Fx/click2.ogg");

    buttonTex = app->tex->Load("Assets/Textures/toggle.png");
	width = 78;
	height = 48;

    boundx = bounds.x;

	toggle = false;
}

GuiToggle::~GuiToggle()
{
    delete buttonTex;
}

bool GuiToggle::Update(float dt)
{

    int camX = app->render->camera.x;
    bounds.x = camX * (-1) + boundx;

	if (state != GuiControlState::DISABLED)
	{
		// L15: TODO 3: Update the state of the GUiButton according to the mouse position

		app->input->GetMousePosition(mouseX, mouseY);

		if (((mouseX - camX) > bounds.x) && ((mouseX - camX) < (bounds.x + bounds.w)) &&
			(mouseY > bounds.y) && (mouseY < (bounds.y + bounds.h)))
		{
			state = GuiControlState::FOCUSED;

			if (previousState != state) {
				app->audio->PlayFx(click1FxId);
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
			{
				if (previousState != state) {
					state = GuiControlState::PRESSED;
					app->audio->PlayFx(click2FxId);
					
				}
			}

			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				//Check if the button can be clicked
				if (canClick)
				{
					NotifyObserver();
					canClick = false;
					if (toggle == false)
						toggle = true;
					else
						toggle = false;
				}
			}
			else {
				canClick = true;
			}

		}
		else {
			state = GuiControlState::NORMAL;
		}

		previousState = state;
	}
	else
		canClick = false;
	return false;

}

bool GuiToggle::Draw(Render* render)
{


	// Draw the right button depending on state
	switch (state)
	{

	case GuiControlState::DISABLED:
	{
		render->DrawRectangle(bounds, 0, 0, 0, 0);
	} break;

	case GuiControlState::NORMAL:
	{
		if (toggle == true) {
			SDL_Rect rect = { 82,0,width,height };
			render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
		}
		else {
			SDL_Rect rect = { 0,0,width,height };
			render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
		}
	} break;

	//L15: TODO 4: Draw the button according the GuiControl State
	case GuiControlState::FOCUSED:
	{
		if (toggle == true) {
			SDL_Rect rect = { 82,100,width,height };
			render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
		}
		else {
			SDL_Rect rect = { 0,100,width,height };
			render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
		}
		


	} break;
	case GuiControlState::PRESSED:
	{
		if (toggle == true) {
			SDL_Rect rect = { 82,50,width,height };
			render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
		}
		else {
			SDL_Rect rect = { 0,50,width,height };
			render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
		}
	} break;

	case GuiControlState::SELECTED:
	{
		render->DrawRectangle(bounds, 0, 255, 0, 255);
		break;
	}

	default:
		break;
	}
	if (state != GuiControlState::DISABLED)
	{
		bounds.x = bounds.x + app->render->camera.x;
	}
	return false;
}