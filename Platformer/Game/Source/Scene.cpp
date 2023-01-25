#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "PathFinding.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "GuiManager.h"
#include "Physics.h"

#include "Defs.h"
#include "Log.h"
#include "LogoScene.h"

Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}



// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	currentLevel = config.child("currentlevel").attribute("value").as_int();
	for (pugi::xml_node levelNode = config.child("level"); levelNode; levelNode = levelNode.next_sibling("level"))
	{
		Level* level = new Level();
		level->id = levelNode.attribute("id").as_int();
		level->name = levelNode.attribute("name").as_string();
		levels.Add(level);
	}

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM, itemNode);
		//item->parameters = itemNode;
	}



	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	bool ret = true;
	// L03: DONE: Load map
	ret = SetUp(currentLevel);

	// Texture to highligh mouse position 
	mouseTileTex = app->tex->Load("Assets/Textures/path.png");
	// Texture to show path origin 
	originTex = app->tex->Load("Assets/Textures/x.png");

	settingsBackground = app->tex->Load("Assets/Textures/SettingsBackground.png");

	uint w, h;
	app->win->GetWindowSize(w, h);


	settingsButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 200, "Settings", { (int)w / 2 - 80,(int)h / 2 - 30,80,24}, this, {0,0,0,0}, 160);

	musicSlider = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 201, "Music Volume", { (int)w / 2 - 320,(int)h / 2 + 160,96,8 }, this, { 8,88,0,0 });
	fxSlider = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 202, "Fx Volume", { (int)w / 2 + 90,(int)h / 2 + 160,96,8 }, this, { 8,88,0,0 });
	musicSlider->state = GuiControlState::OFF;
	fxSlider->state = GuiControlState::OFF;
	musicSlider->SetValue(app->audio->GetMusicVolume());
	fxSlider->SetValue(app->audio->GetFxVolume());

	fullscreenCheckBox = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 203, "Fullscreen", { (int)w / 2 - 300,(int)h / 2 + 210,88,16 }, this, { 0,0,0,0 }, 0);
	vSyncCheckBox = (GuiCheckBox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 204, "Vsync", { (int)w / 2 + 100,(int)h / 2 + 210,88,16 }, this, { 0,0,0,0 }, 1);
	fullscreenCheckBox->state = GuiControlState::OFF;
	vSyncCheckBox->state = GuiControlState::OFF;
	fullscreenCheckBox->SetValue(app->win->fullscreen);
	vSyncCheckBox->SetValue(app->render->vsync);

	quitButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 205, "Quit", { (int)w / 2 - 32,(int)h / 2 - 270,48,24 }, this, { 0,0,0,0 }, 248);

	resumeButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 206, "Resume", { (int)w / 2 - 80,(int)h / 2 - 110,80,24 }, this, { 0,0,0,0 }, 72);

	titleScreenButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 207, "Title", { (int)w / 2 - 80,(int)h / 2 - 190,80,24 }, this, { 0,0,0,0 }, 160);

	return ret;
}

// Called each loop iteration
bool Scene::PreUpdate()
{

	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		player->godMode = !player->godMode;
	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		app->audio->IncreaseVolume();
	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
		app->audio->DecreaseVolume();
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debugMode = !debugMode;
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		app->fadeToBlack->SwitchMap(1);
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		app->fadeToBlack->SwitchMap(2);
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		app->fadeToBlack->SwitchMap(currentLevel);
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		if (showPauseMenu)
			Resume();
		else
			Pause();
	}
		
	if (restartLevel) {
		restartLevel = false;
		app->fadeToBlack->SwitchMap(currentLevel);
	}
	if (nextLevel) {
		nextLevel = false;
		app->fadeToBlack->SwitchMap(currentLevel + 1);
	}

	// Draw map
	app->map->Draw();

	

	

	//Debug
	if (debugMode)
	{
		int mouseX, mouseY;
		app->input->GetMousePosition(mouseX, mouseY);
		iPoint mouseTile = app->map->WorldToMap(mouseX - app->render->camera.x / (int)app->win->GetScale(),
			mouseY - app->render->camera.y / (int)app->win->GetScale());

		//Convert again the tile coordinates to world coordinates to render the texture of the tile
		iPoint highlightedTileWorld = app->map->MapToWorld(mouseTile.x, mouseTile.y);
		//LOG("X: %d Y: %d", highlightedTileWorld.x, highlightedTileWorld.y);
		app->render->DrawTexture(mouseTileTex, highlightedTileWorld.x, highlightedTileWorld.y);

		if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			if (originSelected == true)
			{
				app->pathfinding->CreatePath(origin, mouseTile);
				LOG("X: %d Y: %d", mouseTile.x, mouseTile.y);
				originSelected = false;
			}
			else
			{
				origin = mouseTile;
				LOG("X: %d Y: %d", origin.x, origin.y);
				originSelected = true;

				app->pathfinding->ClearLastPath();
			}
		}
		const DynArray<iPoint>* path = app->pathfinding->GetLastPath();
		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			app->render->DrawTexture(mouseTileTex, pos.x, pos.y);
		}

		// L12: Debug pathfinding
		iPoint originScreen = app->map->MapToWorld(origin.x, origin.y);
		app->render->DrawTexture(originTex, originScreen.x, originScreen.y);

	}
	
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	uint w, h;
	app->win->GetWindowSize(w, h);

	if (showSettings)
	{
		app->render->DrawTexture(settingsBackground, (int(w) / 2 - app->render->camera.x) / app->win->GetScale() - 110, (int(h) / 2 - app->render->camera.y) / app->win->GetScale() + 25);
	}

	app->guiManager->Draw();

	if(quit)
		ret = false;

	

	return ret;
}



// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool Scene::SetUp(int level) 
{
	bool ret = true;

	LOG("SetUp Level = %d", level);

	switch (level)
	{
	case 1:
		currentLevel = 1;
		ret = app->map->Load(levels.At(currentLevel-1)->data->name.GetString());
		break;

	case 2:
		currentLevel = 2;
		ret = app->map->Load(levels.At(currentLevel - 1)->data->name.GetString());
		break;

	default:
		break;
	}

	if (ret) 
	{
		int w, h;
		uchar* data = NULL;

		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);
	}


	return ret;
}

bool Scene::Pause() 
{
	settingsButton->state = GuiControlState::NORMAL;
	resumeButton->state = GuiControlState::NORMAL;
	quitButton->state = GuiControlState::NORMAL;
	titleScreenButton->state = GuiControlState::NORMAL;
	showPauseMenu = true;

	app->physics->active = false;

	return true;
}

bool Scene::Resume()
{
	settingsButton->state = GuiControlState::OFF;
	quitButton->state = GuiControlState::OFF;
	resumeButton->state = GuiControlState::OFF;
	titleScreenButton->state = GuiControlState::OFF;
	musicSlider->state = GuiControlState::OFF;
	fxSlider->state = GuiControlState::OFF;
	fullscreenCheckBox->state = GuiControlState::OFF;
	vSyncCheckBox->state = GuiControlState::OFF;
	showSettings = false;
	showPauseMenu = false;

	app->physics->active = true;

	return true;
}

void Scene::SetGui(bool guiOn) 
{
	if (guiOn) 
	{
		
	}
	else
	{
		settingsButton->state = GuiControlState::OFF;
		quitButton->state = GuiControlState::OFF;
		resumeButton->state = GuiControlState::OFF;
		titleScreenButton->state = GuiControlState::OFF;
		showSettings = false;
		showPauseMenu = false;
	}
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Event by %d ", control->id);

	switch (control->id)
	{
	case 200:
		showSettings = !showSettings;
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
	case 201:
		app->audio->SetMusicVolume(musicSlider->GetValue());
		break;
	case 202:
		app->audio->SetFxVolume(fxSlider->GetValue());
		break;
	case 203:
		app->win->SetFullscreen(fullscreenCheckBox->toggle);
		break;
	case 204:
		app->render->vsync = vSyncCheckBox->toggle;
		break;
	case 205:
		quit = true;
		break;
	case 206:
		Resume();
		break;
	case 207:
		app->fadeToBlack->FadeToBlackScene("TitleScene", 0.2);
		break;
	}

	return true;
}