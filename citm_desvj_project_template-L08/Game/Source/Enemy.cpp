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
	name.Create("Enemy");
	startPosition.x = round(params.attribute("x").as_float());
	startPosition.y = round(params.attribute("y").as_float());
	gid = params.attribute("gid").as_uint();
	GetTextureWithGid();

	SString name = params.attribute("name").as_string();

	parameters = params;
}

Enemy::~Enemy() {

}

bool Enemy::Awake() {

	//L02: DONE 1: Initialize Enemy parameters
	//L02: DONE 5: Get Enemy parameters from XML

	position = startPosition;
	LOG(name.GetString());
	idle.PushBack(GetTileSetWithGid()->GetTileRect(gid));

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

void Enemy::ResetEnemy()
{
	//jump = 0;
	//moveState = MS_IDLE;
	pbody->body->SetLinearVelocity(b2Vec2(0, 0));

	SetPosition(startPosition.x, startPosition.y);
	LOG("load Enemy");
}

void Enemy::SummonEnemy()
{
	pbody = app->physics->CreateRectangle(position.x, position.y, 8, 8, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	pbody->listener = this;

	pbody->ctype = ColliderType::ENEMY;
	pbody->body->SetGravityScale(0);


	//moveState = MS_IDLE;
}

bool Enemy::Update()
{
	FindPath();


	Move();

	if (direction == 0) {
		currentAnimation = &idle;
	}
	if (direction == 1) {
		currentAnimation = &idleleft;
	}
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 4;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 4;

	currentAnimation->Update();
	app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame());

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
		behaviourState = CHASE;
		app->pathfinding->CreatePath(enemyTile, playerTile);
		path = app->pathfinding->GetLastPath();
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
			desiredVel.y = dif.y * speed;
		}
	}
	else
	{
		desiredVel = { 0,0 };
	}



	impulse = desiredVel - vel;
	LOG("X: %f, Y: %f", impulse.x, impulse.y);
	float mass = pbody->body->GetMass();
	pbody->body->ApplyLinearImpulse(impulse, pbody->body->GetWorldCenter(), true);
}

bool Enemy::CleanUp()
{
	return true;
}

bool Enemy::LoadState(pugi::xml_node& data)
{
	b2Vec2 transform;
	transform.x = data.attribute("x").as_int();
	transform.y = data.attribute("y").as_int();
	pbody->body->SetTransform(transform, 0);
	position.x = METERS_TO_PIXELS(transform.x);
	position.y = METERS_TO_PIXELS(transform.y);

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

	data.append_attribute("x") = pbody->body->GetTransform().p.x;
	data.append_attribute("y") = pbody->body->GetTransform().p.y;
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
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	case ColliderType::WALL:
		LOG("Collision WALL");
		break;
	case ColliderType::WATER:
		LOG("Collision WATER");
		break;
	}
}