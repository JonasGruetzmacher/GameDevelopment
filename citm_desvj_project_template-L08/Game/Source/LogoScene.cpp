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
#include "LogoScene.h"

LogoScene::LogoScene() : Module()
{
	name.Create("logoScene");
}

// Destructor
LogoScene::~LogoScene()
{}



// Called before render is available
bool LogoScene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	logoRect.x = config.child("logorect").attribute("x").as_int();
	logoRect.y = config.child("logorect").attribute("y").as_int();
	logoRect.h = config.child("logorect").attribute("h").as_int();
	logoRect.w = config.child("logorect").attribute("w").as_int();

	duration = config.child("duration").attribute("value").as_float();

	texturePath = config.child("path").attribute("value").as_string();
	

	app->scene->active = false;
	app->map->active = false;
	app->physics->active = false;

	return ret;
}

// Called before the first frame
bool LogoScene::Start()
{
	app->fadeToBlack->activeScene = "LogoScene";

	accumulatedTime = 0.0f;

	logo = app->tex->Load(texturePath);

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	return true;
}

// Called each loop iteration
bool LogoScene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool LogoScene::Update(float dt)
{
	app->render->camera.x = 0;
	app->render->camera.y = 0;
		
	//SDL_SetTextureAlphaMod(logo, accumulatedTime * 10.0f);
	app->render->DrawTexture(logo, 0, 0, &logoRect);

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{

		
		
		app->fadeToBlack->DoFadeToBlack(1);
	}
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		app->fadeToBlack->DoFadeToBlack(2);
	}

	return true;
}

// Called each loop iteration
bool LogoScene::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	AutomaticTransition();

	return ret;
}

// Called before quitting
bool LogoScene::CleanUp()
{
	LOG("Freeing LogoScene");

	return true;
}

void LogoScene::AutomaticTransition()
{
	accumulatedTime += 0.075f;

	if (accumulatedTime >= duration)
	{
		app->fadeToBlack->FadeToBlackScene("Scene");
	}
}