#include "App.h"
#include "Textures.h"
#include "Enemy.h"
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

Enemy::Enemy(pugi::xml_node params) : Entity(EntityType::ENEMY)
{
	SString nameID = "Enemy";
	nameID += params.attribute("id").as_string();
	name.Create(nameID.GetString());
	startPosition.x = round(params.attribute("x").as_float());
	startPosition.y = round(params.attribute("y").as_float());
	gid = params.attribute("gid").as_uint();
	GetTextureWithGid();
	moveClass = params.attribute("class").as_string();
	
}

Enemy::~Enemy() {
	
}

bool Enemy::Awake() {

	//L02: DONE 1: Initialize Enemy parameters
	//L02: DONE 5: Get Enemy parameters from XML

	position = startPosition;
	LOG(name.GetString());
	
	tile =  GetTileSetWithGid()->GetTile(gid);
	if (tile != NULL)
	{
		colliderPos = tile->colliderPos;
		colliderHeight = tile->height;
		colliderWidth = tile->width;
		currentAnimation = &tile->animation;
	}
	else
	{
		currentAnimation = &idle;
		idle.PushBack(GetTileSetWithGid()->GetTileRect(gid));
		colliderHeight = 8;
		colliderWidth = 8;
		colliderPos = { 0,0 };
	}

	return true;
}

bool Enemy::Start() {
	SummonEnemy();
	
	return true;
}

bool Enemy::SetPosition(int x, int y)
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


void Enemy::SummonFlyingEnemy()
{
	pbody = app->physics->CreateRectangle(position.x + colliderPos.x, position.y + colliderPos.y, colliderWidth, colliderHeight, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	pbody->listener = this;

	pbody->ctype = ColliderType::ENEMY;
	pbody->body->SetGravityScale(0);


	//moveState = MS_IDLE;
}

void Enemy::SummonEnemy() 
{
	if (moveClass == "Walking") 
	{
		SummonWalkingEnemy();
	}
	else if (moveClass == "Flying")
	{
		SummonFlyingEnemy();
	} else 
	{
		LOG("No Enemy Class assigned. Problems might occur!");
		SummonFlyingEnemy();
	}
}

void Enemy::SummonWalkingEnemy()
{
	pbody = app->physics->CreateRectangle(position.x + colliderPos.x, position.y + colliderPos.y, colliderWidth, colliderHeight, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	pbody->listener = this;

	pbody->ctype = ColliderType::ENEMY;


	//moveState = MS_IDLE;
}

bool Enemy::Update()
{
	FindPath();
	Move();
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 4;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 4;

	currentAnimation->Update();
	app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame(), !right);

	return true;
}

void Enemy::FindPath()
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

void Enemy::Jump() {
	if (!isJumping) {
		pbody->body->SetLinearVelocity(b2Vec2(0, 0));
		float mass = pbody->body->GetMass();
		pbody->body->ApplyLinearImpulse(b2Vec2(0, -30 * mass), pbody->body->GetWorldCenter(), true);

		isJumping = true;
	}
}

void Enemy::Move() 
{
	b2Vec2 vel = pbody->body->GetLinearVelocity();
	b2Vec2 desiredVel = { 0,0 };
	b2Vec2 impulse;
	
	if (behaviourState == CHASE) {
		if (path->Count() > 1) {
			const iPoint* tile = path->At(0);
			const iPoint* nextTile = path->At(1);

			b2Vec2 dif = { (float)nextTile->x - tile->x , (float)nextTile->y - tile->y };
			dif.Normalize();
			desiredVel.x = dif.x * speed;
			if (moveClass == "Flying") {
				desiredVel.y = dif.y * speed;
			}
			else if(dif.y < 0) {
				//Jump();
			}
			
		}
	}
	else
	{
		desiredVel = { 0,0 };
	}
	if (desiredVel.x > 0)
	{
		right = true;
	}
	else if (desiredVel.x < 0)
	{
		right = false;
	}


	impulse = desiredVel - vel;
	if (moveClass == "Walking") {
		impulse.y = 0;
	}
	float mass = pbody->body->GetMass();
	pbody->body->ApplyLinearImpulse(impulse, pbody->body->GetWorldCenter(), true);
}

bool Enemy::CleanUp()
{
	app->physics->world;
	pbody->body->GetWorld()->DestroyBody(pbody->body);
	//delete pbody;
	return true;
}

bool Enemy::LoadState(pugi::xml_node& data)
{
	position.x = data.attribute("x").as_int();
	position.y = data.attribute("y").as_int();	
	
	active = true;
	SummonEnemy();

	b2Vec2 vel;
	vel.x = data.child("velocity").attribute("x").as_int();
	vel.y = data.child("velocity").attribute("y").as_int();
	pbody->body->SetLinearVelocity(vel);
	LOG("load Enemy");
	return true;
}

// L03: DONE 8: Create a method to save the state of the renderer
// using append_child and append_attribute
bool Enemy::SaveState(pugi::xml_node& data)
{
	//pugi::xml_node Enemy = data.append_child("Enemy");

	data.append_attribute("x") = position.x;
	data.append_attribute("y") = position.y;
	data.append_child("velocity");
	data.child("velocity").append_attribute("x") = pbody->body->GetLinearVelocity().x;
	data.child("velocity").append_attribute("y") = pbody->body->GetLinearVelocity().y;

	return true;
}

void Enemy::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		isJumping = false;
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		isJumping = false;
		break;
	case ColliderType::WALL:
		LOG("Collision WALL");
		break;
	case ColliderType::WATER:
		LOG("Collision WATER");
		isJumping = false;
		break;
	}
}