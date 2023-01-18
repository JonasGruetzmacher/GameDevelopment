#include "GuiButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"
#include "Window.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

	//audioFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");
}

GuiButton::~GuiButton()
{

}

bool GuiButton::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		// L15: TODO 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		GuiControlState previousState = state;

		// I'm inside the limitis of the button
		if (mouseX * app->win->GetScale() >= bounds.x && mouseX * app->win->GetScale() <= bounds.x + bounds.w &&
			mouseY * app->win->GetScale() >= bounds.y && mouseY * app->win->GetScale() <= bounds.y + bounds.h) {

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
		render->DrawRectangle(bounds, 200, 200, 200, 255, true, false);
		break;
	case GuiControlState::NORMAL:
		render->DrawRectangle(bounds, 0, 0, 255, 255, true, false);
		break;
	case GuiControlState::FOCUSED:
		render->DrawRectangle(bounds, 0, 0, 20, 255, true, false);
		break;
	case GuiControlState::PRESSED:
		render->DrawRectangle(bounds, 0, 255, 0, 255, true, false);
		break;
	}

	app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h, { 255,255,255 });

	return false;
}