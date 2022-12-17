#include <math.h>

#include "App.h"
#include "Module.h"
#include "FadeToBlack.h"
#include "Window.h"
#include "Log.h"
#include "Render.h"
#include "EntityManager.h"
#include "Map.h"
#include "Scene.h"
#include "LogoScene.h"
#include "TitleScene.h"
#include "External/SDL/include/SDL_render.h"
#include "External/SDL/include/SDL_timer.h"
#include "Physics.h"

FadeToBlack::FadeToBlack()
{
	name.Create("fade");

}

FadeToBlack::~FadeToBlack()
{}

bool FadeToBlack::Awake(pugi::xml_node& conf)
{
	uint width, height;
	app->win->GetWindowSize(width, height);
	screen.w = width;
	screen.h = height;
	screen.x = screen.y = 0;

	return true;
}

// Load assets
bool FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);

	fadingPlayer = false;
	sceneSwitch = false;
	wantToSwitchScene = "LogoScene";

	app->scene->CleanUp();
	app->scene->active = false;
	app->map->active = false;
	app->physics->active = false;
	app->logoScene->active = true;
	app->titleScene->active = false;
	app->entityManager->active = false;

	return true;
}

// Update: draw background
bool FadeToBlack::Update(float dt)
{

	if (currentStep == FadeStep::NONE)
		return true;

	Uint32 now = SDL_GetTicks() - startTime;
	float normalized = MIN(1.0f, (float)now / (float)totalTime);

	switch (currentStep)
	{
	case FadeStep::FADE_TO:
	{
		if (now >= totalTime)
		{
			if (!sceneSwitch)
			{
				//app->scene->player->ResetPlayer();
			}
			else
			{
				SwitchScenes(wantToSwitchScene);
			}
			totalTime += totalTime;
			startTime = SDL_GetTicks();
			currentStep = FadeStep::FADE_FROM;
		}
	} break;

	case FadeStep::FADE_FROM:
	{
		normalized = 1.0f - normalized;

		if (now >= totalTime)
			currentStep = FadeStep::NONE;

		fadingPlayer = false;
		sceneSwitch = false;
	} break;
	}

	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(app->render->renderer, &screen);

	return true;
}

// Fade to black. At mid point deactivate one module, then activate the other
bool FadeToBlack::DoFadeToBlack(int level, float time)
{
	bool ret = false;

	nextLevel = level;

	if (currentStep == FadeStep::NONE)
	{
		currentStep = FadeStep::FADE_TO;
		startTime = SDL_GetTicks();
		totalTime = (Uint32)(time * 0.5f * 1000.0f);
		ret = true;

		app->scene->player->active = true;

	}

	return ret;
}





bool FadeToBlack::FadeToBlackScene(char* scene, float time)
{
	bool ret = false;

	sceneSwitch = true;
	wantToSwitchScene = scene;

	if (currentStep == FadeStep::NONE)
	{
		currentStep = FadeStep::FADE_TO;
		startTime = SDL_GetTicks();
		totalTime = (Uint32)(time * 0.5f * 1000.0f);
		ret = true;
	}

	return ret;
}

bool FadeToBlack::SwitchMap(int level)
{
	return false;
}

bool FadeToBlack::SwitchScenes(char* scene)
{
	if (activeScene != scene)
	{
		
		if (scene == "LogoScene")
		{
			
			app->entityManager->CleanUp();
			app->scene->CleanUp();
			app->scene->active = false;
			app->map->active = false;
			app->physics->active = false;
			app->logoScene->active = true;
			app->titleScene->active = false;
			app->entityManager->active = false;
		}
		if (scene == "TitleScene")
		{
			app->scene->active = false;
			app->map->active = false;
			app->physics->active = false;
			app->logoScene->active = false;
			app->titleScene->active = true;
		}
		if (scene == "Scene")
		{
			app->scene->active = true;
			app->logoScene->active = false;
			app->map->active = true;
			app->physics->active = true;
			app->titleScene->active = false;
			app->entityManager->active = true;
		}

		activeScene = scene;
	}

	return true;
}

