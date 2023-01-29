#include "App.h"
#include "Textures.h"
#include "Coins.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Window.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Map.h"
#include "PathFinding.h"

Coin::Coin(pugi::xml_node params) : Entity(EntityType::COIN)
{
	SString nameID = "Coin";
	nameID += params.attribute("id").as_string();
	name.Create(nameID.GetString());
	startPosition.x = round(params.attribute("x").as_float());
	startPosition.y = round(params.attribute("y").as_float());
	gid = params.attribute("gid").as_uint();
	GetTextureWithGid();
	moveClass = params.attribute("class").as_string();

}

Coin::~Coin() {

}

bool Coin::Awake() {

	//L02: DONE 1: Initialize Enemy parameters
	//L02: DONE 5: Get Enemy parameters from XML

	position = startPosition;
	LOG(name.GetString());

	tile = GetTileSetWithGid()->GetTile(gid);
	if (tile != NULL)
	{
		colliderPos = tile->colliderPos;
		colliderHeight = tile->height;
		colliderWidth = tile->width;
		//currentAnimation = &tile->animation;
	}
	else
	{
		/*  currentAnimation = &idle;
			idle.PushBack(GetTileSetWithGid()->GetTileRect(gid));
			colliderHeight = 8;
			colliderWidth = 8;
			colliderPos = { 0,0 };*/
	}

	return true;
}

bool Coin::Start() {
	SummonCoin();

	return true;
}

bool Coin::SetPosition(int x, int y)
{
	bool ret = true;

	b2Vec2 transform;
	transform.x = PIXEL_TO_METERS(x);
	transform.y = PIXEL_TO_METERS(y);
	pbody->body->SetTransform(transform, 0);
	position.x = METERS_TO_PIXELS(transform.x);
	position.y = METERS_TO_PIXELS(transform.y);

	return ret;
}

void Coin::SummonCoin()
{
	pbody = app->physics->CreateRectangle(position.x + colliderPos.x, position.y + colliderPos.y, colliderWidth, colliderHeight, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	pbody->listener = this;

	pbody->ctype = ColliderType::COIN;


}
/*
bool Coin::Update()
{
	FindPath();
	Move();
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 4;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 4;

	currentAnimation->Update();
	app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame(), !right);

	return true;
}*/
/*
void Coin::FindPath()
{
	iPoint playerTile = app->map->WorldToMap(app->scene->player->position.x, app->scene->player->position.y);
	iPoint enemyTile = app->map->WorldToMap(position.x, position.y);
	int distanceToPlayer = playerTile.DistanceTo(enemyTile);
	if (distanceToPlayer > 10)
	{
		behaviourState = IDLE;
	}
	else {

		if (app->pathfinding->CreatePath(enemyTile, playerTile) != -1) {
			behaviourState = CHASE;
			path = app->pathfinding->GetLastPath();
		}
		else {
			behaviourState = IDLE;
		}
	}
}
*/

bool Coin::CleanUp()
{
	app->physics->world;
	pbody->body->GetWorld()->DestroyBody(pbody->body);
	return true;
}

bool Coin::LoadState(pugi::xml_node& data)
{
	position.x = data.attribute("x").as_int();
	position.y = data.attribute("y").as_int();

	active = true;
	SummonCoin();

	b2Vec2 vel;
	vel.x = data.child("velocity").attribute("x").as_int();
	vel.y = data.child("velocity").attribute("y").as_int();
	pbody->body->SetLinearVelocity(vel);
	LOG("load Coin");
	return true;
}

// L03: DONE 8: Create a method to save the state of the renderer
// using append_child and append_attribute
bool Coin::SaveState(pugi::xml_node& data)
{
	//pugi::xml_node Enemy = data.append_child("Enemy");

	data.append_attribute("x") = position.x;
	data.append_attribute("y") = position.y;
	//data.append_child("velocity");
	//data.child("velocity").append_attribute("x") = pbody->body->GetLinearVelocity().x;
	//data.child("velocity").append_attribute("y") = pbody->body->GetLinearVelocity().y;

	return true;
}

void Coin::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

	switch (physB->ctype)
	{ 
		/*  
	 case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		Unstuck(physB->body->GetPosition().x, physB->body->GetPosition().y);
		isJumping = false;
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		Unstuck(physB->body->GetPosition().x, physB->body->GetPosition().y);
		isJumping = false;
		break;
	case ColliderType::WALL:
		LOG("Collision WALL");
		Unstuck(physB->body->GetPosition().x, physB->body->GetPosition().y);
		break;
	case ColliderType::WATER:
		LOG("Collision WATER");
		Unstuck(physB->body->GetPosition().x, physB->body->GetPosition().y);
		isJumping = false;
		break;
	}*/
	case ColliderType::PLAYER:
		LOG("Collision ITEM");
		break;
}
