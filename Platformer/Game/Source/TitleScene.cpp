#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "Physics.h"
#include "FadeToBlack.h"
#include "GuiManager.h"

#include "Defs.h"
#include "Log.h"
#include "TitleScene.h"
#include "Chars.h"

TitleScene::TitleScene() : Module()
{
	name.Create("titleScene");
}

// Destructor
TitleScene::~TitleScene()
{}



// Called before render is available
bool TitleScene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	
	logoRect.x = config.child("logorect").attribute("x").as_int();
	logoRect.y = config.child("logorect").attribute("y").as_int();
	logoRect.h = config.child("logorect").attribute("h").as_int();
	logoRect.w = config.child("logorect").attribute("w").as_int();
	

	texturePath = config.child("path").attribute("value").as_string();

	return ret;
}

// Called before the first frame
bool TitleScene::Start()
{
	app->fadeToBlack->activeScene = "TitleScene";

	logo = app->tex->Load(texturePath);
	settingsBackground = app->tex->Load("Assets/Textures/SettingsBackground.png");
	creditsTex = app->tex->Load("Assets/Textures/Credits.png");

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	uint w, h;
	app->win->GetWindowSize(w, h);

	playButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Play", { (int)w / 2 - 280,(int)h / 2 - 160,64,24 }, this,{0,0,0,0}, 0);

	continueButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Continue", { (int)w / 2 - 305,(int)h / 2 - 80,80,24 }, this,{0,0,0,0}, 72);

	settingsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Settings", { (int)w / 2 - 305,(int)h / 2,80,24 }, this, {0,0,0,0}, 160);

	quitButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 8, "Quit", { (int)w / 2 - 255,(int)h / 2 - 270,48,24 }, this, { 0,0,0,0 }, 248);

	creditsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 9, "Credits", { (int)w / 2 + 200,(int)h / 2 - 270,80,24 }, this, { 0,0,0,0 }, 300);

	musicSlider = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 4, "Music Volume", { (int)w / 2 - 320,(int)h / 2 + 160,96,8 }, this, { 8,88,0,0 });
	fxSlider = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 5, "Fx Volume", { (int)w / 2 + 90,(int)h / 2 + 160,96,8 }, this, { 8,88,0,0 });
	musicSlider->state = GuiControlState::OFF;
	fxSlider->state = GuiControlState::OFF;
	musicSlider->SetValue(app->audio->GetMusicVolume());
	fxSlider->SetValue(app->audio->GetFxVolume());
	
	fullscreenCheckBox = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 6, "Fullscreen", { (int)w / 2 - 300,(int)h / 2 + 210,88,16 }, this, { 0,0,0,0 }, 0);
	vSyncCheckBox = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 7, "Vsync", { (int)w / 2 + 100,(int)h / 2 + 210,88,16 }, this, { 0,0,0,0 }, 1);
	fullscreenCheckBox->state = GuiControlState::OFF;
	vSyncCheckBox->state = GuiControlState::OFF;
	fullscreenCheckBox->SetValue(app->win->fullscreen);
	vSyncCheckBox->SetValue(app->render->vsync);

	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

	if (result == NULL) 
	{
		continueButton->state = GuiControlState::DISABLED;
	}

	return true;
}

// Called each loop iteration
bool TitleScene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool TitleScene::Update(float dt)
{
	app->render->camera.x = 0;
	app->render->camera.y = 0;
		
	uint w, h;
	app->win->GetWindowSize(w, h);
	app->render->DrawTexture(logo, 0, 0, &logoRect);

	
	if (showSettings) 
	{
		app->render->DrawTexture(settingsBackground, int(w)/2 / app->win->GetScale() - 110, int(h) / 2 / app->win->GetScale() + 25);
	}
	if (showCredits)
	{
		app->render->DrawTexture(creditsTex, int(w) / 2 / app->win->GetScale(), int(h) / 2 / app->win->GetScale() - 50);
	}

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		app->fadeToBlack->FadeToBlackScene("Scene", 0.1);
	}
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		app->fadeToBlack->DoFadeToBlack(2);
	}

	app->guiManager->Draw();

	return true;
}

// Called each loop iteration
bool TitleScene::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || quit)
		ret = false;


	return ret;
}

// Called before quitting
bool TitleScene::CleanUp()
{
	LOG("Freeing TitleScene");

	return true;
}

void TitleScene::SetGui(bool guiOn) 
{
	if (guiOn)
	{
		playButton->state = GuiControlState::NORMAL;
		continueButton->state = GuiControlState::NORMAL;
		settingsButton->state = GuiControlState::NORMAL;
		creditsButton->state = GuiControlState::NORMAL;
		quitButton->state = GuiControlState::NORMAL;
		pugi::xml_document gameStateFile;
		pugi::xml_parse_result result = gameStateFile.load_file("save_game.xml");

		if (result == NULL)
		{
			continueButton->state = GuiControlState::DISABLED;
		}
	}
	else
	{
		playButton->state = GuiControlState::OFF;
		continueButton->state = GuiControlState::OFF;
		settingsButton->state = GuiControlState::OFF;
		creditsButton->state = GuiControlState::OFF;
		quitButton->state = GuiControlState::OFF;
		musicSlider->state = GuiControlState::OFF;
		fxSlider->state = GuiControlState::OFF;
		fullscreenCheckBox->state = GuiControlState::OFF;
		vSyncCheckBox->state = GuiControlState::OFF;
		showSettings = false;
	}
}

bool TitleScene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Event by %d ", control->id);

	bool ret = true;
	switch (control->id)
	{
	case 1:
		LOG("Button Play click");
		app->fadeToBlack->FadeToBlackScene("Scene", 0.2);
		break;
	case 2:
		LOG("Button Continue click");
		app->LoadFromFile();
		app->fadeToBlack->FadeToBlackScene("Scene", 0.2);
		app->scene->load = true;
		break;
	case 3:
		showSettings = !showSettings;
		showCredits = false;
		if (!showSettings)
		{
			musicSlider->state = GuiControlState::OFF;
			fxSlider->state = GuiControlState::OFF;
			fullscreenCheckBox->state = GuiControlState::OFF;
			vSyncCheckBox->state = GuiControlState::OFF;
		}
		else
		{
			musicSlider->state = GuiControlState::NORMAL;
			fxSlider->state = GuiControlState::NORMAL;
			fullscreenCheckBox->state = GuiControlState::NORMAL;
			vSyncCheckBox->state = GuiControlState::NORMAL;
		}
		break;
	case 4:
		app->audio->SetMusicVolume(musicSlider->GetValue());
		break;
	case 5:
		app->audio->SetFxVolume(fxSlider->GetValue());
		break;
	case 6:
		app->win->SetFullscreen(fullscreenCheckBox->toggle);
		break;
	case 7:
		app->render->vsync = vSyncCheckBox->toggle;
		break;
	case 8:
		quit = true;
		break;
	case 9 :
		showCredits = !showCredits;
		showSettings = false;
		musicSlider->state = GuiControlState::OFF;
		fxSlider->state = GuiControlState::OFF;
		fullscreenCheckBox->state = GuiControlState::OFF;
		vSyncCheckBox->state = GuiControlState::OFF;
		break;
	}

	return ret;
}

