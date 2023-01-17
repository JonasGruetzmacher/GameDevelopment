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

#include "Defs.h"
#include "Log.h"
#include "TitleScene.h"

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

	app->render->camera.x = 0;
	app->render->camera.y = 0;

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
		
	//SDL_SetTextureAlphaMod(logo, accumulatedTime * 10.0f);
	app->render->DrawTexture(logo, 0, 0);

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{

		
		
		app->fadeToBlack->FadeToBlackScene("Scene", 0.1);
	}
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		app->fadeToBlack->DoFadeToBlack(2);
	}

	return true;
}

// Called each loop iteration
bool TitleScene::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;


	return ret;
}

// Called before quitting
bool TitleScene::CleanUp()
{
	LOG("Freeing TitleScene");

	return true;
}

