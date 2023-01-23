#include "GuiButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"
#include "Window.h"
#include "Textures.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text, SDL_Texture* tex, int rectX) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
	this->tex = tex;
	this->rectX = rectX;

	canClick = true;
	drawBasic = false;

	//audioFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");
}

GuiButton::~GuiButton()
{

}

bool GuiButton::Update(float dt)
{
	if (state != GuiControlState::DISABLED && state != GuiControlState::OFF)
	{
		// L15: TODO 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		GuiControlState previousState = state;

		// I'm inside the limitis of the button
		if (mouseX * app->win->GetScale()  >= bounds.x && mouseX * app->win->GetScale() <= bounds.x + bounds.w * app->win->GetScale() &&
			mouseY * app->win->GetScale() >= bounds.y && mouseY * app->win->GetScale() <= bounds.y + bounds.h * app->win->GetScale()) {

			state = GuiControlState::FOCUSED;
			if (previousState != state) {
				LOG("Change state from %d to %d", previousState, state);
				//app->audio->PlayFx(audioFxId);
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
				state = GuiControlState::PRESSED;
			}

			//
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {
				NotifyObserver();
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}

	}

	return false;
}

bool GuiButton::Draw(Render* render)
{
	//L15: TODO 4: Draw the button according the GuiControl State
	switch (state)
	{
	case GuiControlState::DISABLED:

		//SDL_Rect rect = { 0,0,190,66 };
		//render->DrawTexture(buttonTex, bounds.x, bounds.y, &rect);
		if (tex == nullptr)
		{
			render->DrawRectangle(bounds, 200, 200, 200, 255, true, false);
		}
		else 
		{
			render->DrawTexture(tex, bounds.x / app->win->GetScale(), bounds.y / app->win->GetScale(), new SDL_Rect{ rectX, (bounds.h + 8) * 1,bounds.w,bounds.h });
		}
		break;
	case GuiControlState::NORMAL:
		if (tex == nullptr)
		{
			render->DrawRectangle(bounds, 0, 0, 255, 255, true, false);
		}
		else
		{
			render->DrawTexture(tex, bounds.x/app->win->GetScale(), bounds.y / app->win->GetScale(), new SDL_Rect{ rectX,0,bounds.w,bounds.h});
		}
		break;
	case GuiControlState::FOCUSED:
		if (tex == nullptr)
		{
			render->DrawRectangle(bounds, 0, 0, 20, 255, true, false);
		}
		else
		{
			render->DrawTexture(tex, bounds.x / app->win->GetScale(), bounds.y / app->win->GetScale(), new SDL_Rect{ rectX, (bounds.h + 8) * 2,bounds.w,bounds.h });
		}
		break;
	case GuiControlState::PRESSED:
		if (tex == nullptr)
		{
			render->DrawRectangle(bounds, 0, 255, 0, 255, true, false);
		}
		else
		{
			render->DrawTexture(tex, bounds.x / app->win->GetScale(), bounds.y / app->win->GetScale(), new SDL_Rect{ rectX, (bounds.h + 8) * 3,bounds.w,bounds.h });
		}
		break;
	case GuiControlState::OFF:
		break;
	}

	if (state != GuiControlState::OFF && tex == nullptr)
	{
		app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w * app->win->GetScale(), bounds.h * app->win->GetScale(), { 255,255,255 });
	}
	return false;
}