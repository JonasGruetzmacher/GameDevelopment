#include "EntityManager.h"
#include "Player.h"
#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Scene.h"
#include "Enemy.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	

	return ret;

}

bool EntityManager::Start() {


	bool ret = true; 

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	//Iterates over the entities and calls Start
	//ListItem<Entity*>* item;
	//Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type, pugi::xml_node parameters)
{
	Entity* entity = nullptr; 

	//L02: DONE 2: Instantiate entity according to the type and add the new entoty it to the list of Entities

	switch (type)
	{

	case EntityType::PLAYER:
		entity = new Player(parameters);
		break;

	case EntityType::ITEM:
		entity = new Item(parameters);
		break;
	case EntityType::ENEMY:
		entity = new Enemy(parameters);
		break;
	default: break;
	}

	// Created entities are added to the list
	AddEntity(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) {
			
			item->data->toDestroy = true;
			//delte item->data;
		}
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

bool EntityManager::Update(float dt)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		if (pEntity->toDestroy == true) {
			entities.Del(item);
			item->data->CleanUp();
		}
		ret = item->data->Update();
	}

	return ret;
}

bool EntityManager::SaveState(pugi::xml_node& data) {

	bool ret = true;

	pugi::xml_node cam = data.append_child("entities");

	ListItem<Entity*>* item;
	item = entities.start;
	
	while (item != NULL)
	{
		ret = item->data->SaveState(data.child("entities").append_child(item->data->name.GetString()));
		item = item->next;
	}

	return ret;
}

bool EntityManager::LoadState(pugi::xml_node& data) {
	bool ret = true;

	ListItem<Entity*>* item;
	item = entities.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->LoadState(data.child("entities").child(item->data->name.GetString()));
		item = item->next;
	}

	return ret;
}