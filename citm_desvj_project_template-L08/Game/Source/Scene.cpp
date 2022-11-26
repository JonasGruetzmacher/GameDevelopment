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

	// L04: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	app->win->SetTitle(title.GetString());

	// Texture to highligh mouse position 
	mouseTileTex = app->tex->Load("Assets/Textures/path.png");
	// Texture to show path origin 
	originTex = app->tex->Load("Assets/Textures/x.png");

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
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		player->godMode = !player->godMode;
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		app->audio->IncreaseVolume();
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		app->audio->DecreaseVolume();
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debugMode = !debugMode;

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

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
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
