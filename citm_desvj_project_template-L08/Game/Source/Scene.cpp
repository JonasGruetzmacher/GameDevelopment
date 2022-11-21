#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
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
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	//L02: DONE 3: Instantiate the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");

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

	// Draw map
	app->map->Draw();

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
		ret = app->map->Load("Assets/Maps/NewMap.tmx");
		break;

	default:
		break;
	}

	return ret;
}
