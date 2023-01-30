#include "GuiCheckBox.h"
#include "Render.h"
#include "App.h"
#include "Window.h"
#include "Audio.h"
#include "Textures.h"
#include "Timer.h"
#include "Render.h"
#include "Log.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text, int texId) : GuiControl(GuiControlType::CHECKBOX, id)
{
    this->bounds = bounds;
    this->text = text;
	this->texId = texId;

    canClick = true;
    drawBasic = false;

    //Load Button Click Sounds
	click1FxId = 7;
	click2FxId = 8;

    buttonTex = app->tex->Load("Assets/Textures/CheckBoxButtons.png");
	checkBoxTex = app->tex->Load("Assets/Textures/CheckBox.png");

    boundx = bounds.x;

	toggle = false;
}

GuiCheckBox::~GuiCheckBox()
{
    delete buttonTex;
}

bool GuiCheckBox::Update(float dt)
{

	if (state != GuiControlState::DISABLED && state != GuiControlState::OFF)
	{
		// L15: TODO 3: Update the state of the GUiButton according to the mouse position

		app->input->GetMousePosition(mouseX, mouseY);

		if (mouseX * app->win->GetScale() >= bounds.x && mouseX * app->win->GetScale() <= bounds.x + bounds.w * app->win->GetScale() &&
			mouseY * app->win->GetScale() >= bounds.y && mouseY * app->win->GetScale() <= bounds.y + bounds.h * app->win->GetScale())
		{
			state = GuiControlState::FOCUSED;

			if (previousState != state) {
				app->audio->PlayFx(click1FxId);
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
			{
				if (previousState != state) {
					state = GuiControlState::PRESSED;
					
					
				}
			}

			// If mouse button pressed -> Generate event!
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP)
			{
				//Check if the button can be clicked
				if (canClick)
				{
					app->audio->PlayFx(click2FxId);
					toggle = !toggle;
					NotifyObserver();
					canClick = false;
					
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
	return true;

}

bool GuiCheckBox::Draw(Render* render)
{
	//L15: TODO 4: Draw the button according the GuiControl State
	switch (state)
	{
	case GuiControlState::DISABLED:
		render->DrawTexture(buttonTex, (bounds.x-app->render->camera.x) / app->win->GetScale(), (bounds.y - app->render->camera.y) / app->win->GetScale(), new SDL_Rect{ texId * 80, 24 * 1,72,16 });
		if (toggle)
		{
			app->render->DrawTexture(checkBoxTex, (bounds.x - app->render->camera.x) / app->win->GetScale() + 76, (bounds.y - app->render->camera.y) / app->win->GetScale(), new SDL_Rect{ 24, 24 * 1, 16, 16 });
		}
		else
		{
			app->render->DrawTexture(checkBoxTex, (bounds.x - app->render->camera.x) / app->win->GetScale() + 76, (bounds.y - app->render->camera.y) / app->win->GetScale(), new SDL_Rect{ 0, 24 * 1, 16, 16 });
		}
		break;
	case GuiControlState::NORMAL:
		render->DrawTexture(buttonTex, (bounds.x - app->render->camera.x) / app->win->GetScale(), (bounds.y - app->render->camera.y) / app->win->GetScale(), new SDL_Rect{ texId * 80, 0, 72, 16 });
		if (toggle)
		{
			app->render->DrawTexture(checkBoxTex, (bounds.x - app->render->camera.x) / app->win->GetScale() + 76, (bounds.y - app->render->camera.y) / app->win->GetScale(), new SDL_Rect{ 24, 0, 16, 16 });
		}
		else
		{
			app->render->DrawTexture(checkBoxTex, (bounds.x - app->render->camera.x) / app->win->GetScale() + 76, (bounds.y - app->render->camera.y) / app->win->GetScale(), new SDL_Rect{ 0, 0, 16, 16 });
		}
		break;
	case GuiControlState::FOCUSED:
		render->DrawTexture(buttonTex, (bounds.x - app->render->camera.x) / app->win->GetScale(), (bounds.y - app->render->camera.y) / app->win->GetScale(), new SDL_Rect{ texId * 80, 24 * 2, 72, 16 });
		if (toggle)
		{
			app->render->DrawTexture(checkBoxTex, (bounds.x - app->render->camera.x) / app->win->GetScale() + 76, (bounds.y - app->render->camera.y) / app->win->GetScale(), new SDL_Rect{ 24, 24 * 2, 16, 16 });
		}
		else
		{
			app->render->DrawTexture(checkBoxTex, (bounds.x - app->render->camera.x) / app->win->GetScale() + 76, (bounds.y - app->render->camera.y) / app->win->GetScale(), new SDL_Rect{ 0, 24 * 2, 16, 16 });
		}
		break;
	case GuiControlState::PRESSED:
		render->DrawTexture(buttonTex, (bounds.x - app->render->camera.x) / app->win->GetScale(), (bounds.y - app->render->camera.y) / app->win->GetScale(), new SDL_Rect{ texId * 80, 24 * 3, 72 , 16 });
		if (toggle)
		{
			app->render->DrawTexture(checkBoxTex, (bounds.x - app->render->camera.x) / app->win->GetScale() + 76, (bounds.y - app->render->camera.y) / app->win->GetScale(), new SDL_Rect{ 24, 24 * 3, 16, 16 });
		}
		else
		{
			app->render->DrawTexture(checkBoxTex, (bounds.x - app->render->camera.x) / app->win->GetScale() + 76, (bounds.y - app->render->camera.y) / app->win->GetScale(), new SDL_Rect{ 0, 24 * 3, 16, 16 });
		}
		break;
	case GuiControlState::OFF:
		break;
	}
	return true;
}

void GuiCheckBox::SetValue(bool toggle)
{
	this->toggle = toggle;
}

bool GuiCheckBox::GetValue()
{
	return toggle;
}