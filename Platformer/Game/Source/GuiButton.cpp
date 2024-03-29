#include "GuiButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"
#include "Window.h"
#include "Textures.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, int rectX) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
	this->rectX = rectX;

	canClick = true;
	drawBasic = false;

	tex = app->tex->Load("Assets/Textures/buttons.png");

	audioFxId1 = 7;
	audioFxId2 = 8;
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
			if (previousState == GuiControlState::NORMAL)
				app->audio->PlayFx(audioFxId1);
			if (previousState != state) {
				LOG("Change state from %d to %d", previousState, state);
				
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
				state = GuiControlState::PRESSED;
			}

			//
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {
				NotifyObserver();
				app->audio->PlayFx(audioFxId2);
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
		render->DrawTexture(tex, (bounds.x - app->render->camera.x)/ app->win->GetScale(), (bounds.y - app->render->camera.y) / app->win->GetScale(), new SDL_Rect{ rectX, (bounds.h + 8) * 1,bounds.w,bounds.h });
		break;
	case GuiControlState::NORMAL:
		//render->DrawTexture(tex, bounds.x/app->win->GetScale(), bounds.y / app->win->GetScale(), new SDL_Rect{ rectX,0,bounds.w,bounds.h});
		render->DrawTexture(tex, (bounds.x - app->render->camera.x) / app->win->GetScale(), (bounds.y - app->render->camera.y) /app->win->GetScale(), new SDL_Rect{rectX,0,bounds.w,bounds.h});
		break;
	case GuiControlState::FOCUSED:
		render->DrawTexture(tex, (bounds.x - app->render->camera.x) / app->win->GetScale(), (bounds.y - app->render->camera.y) / app->win->GetScale(), new SDL_Rect{ rectX, (bounds.h + 8) * 2,bounds.w,bounds.h });
		break;
	case GuiControlState::PRESSED:
		render->DrawTexture(tex, (bounds.x - app->render->camera.x) / app->win->GetScale(), (bounds.y - app->render->camera.y) / app->win->GetScale(), new SDL_Rect{ rectX, (bounds.h + 8) * 3,bounds.w,bounds.h });
		break;
	case GuiControlState::OFF:
		break;
	}
	return true;
}