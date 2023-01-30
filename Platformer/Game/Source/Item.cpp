#include "Item.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Map.h"

Item::Item(pugi::xml_node params) : Entity(EntityType::ITEM)
{
	SString nameID = "Item";
	nameID += params.attribute("id").as_string();
	name.Create(nameID.GetString());
	startPosition.x = round(params.attribute("x").as_float());
	startPosition.y = round(params.attribute("y").as_float());
	gid = params.attribute("gid").as_uint();
	GetTextureWithGid();
	itemClass = params.attribute("class").as_string();
}

Item::~Item() {}

bool Item::Awake() {

	position = startPosition;

	tile = GetTileSetWithGid()->GetTile(gid);
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

bool Item::Start() {

	SummonItem();


	return true;
}

bool Item::SetPosition(int x, int y)
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

void Item::SummonItem()
{
	if (itemClass == "Coin")
	{
		SummonCoin();
	}
	else if (itemClass == "Heart")
	{
		SummonHeart();
	}
	else if (itemClass == "Ammo")
	{
		SummonAmmo();
	}
	else
	{
		LOG("No Item Class assigned.");
	}
}

void Item::SummonCoin()
{
	pbody = app->physics->CreateRectangle(position.x + colliderPos.x, position.y + colliderPos.y, colliderWidth, colliderHeight, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	pbody->listener = this;

	pbody->ctype = ColliderType::COIN;
}

void Item::SummonHeart()
{
	pbody = app->physics->CreateRectangle(position.x + colliderPos.x, position.y + colliderPos.y, colliderWidth, colliderHeight, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	pbody->listener = this;

	pbody->ctype = ColliderType::HEALTH;
}

void Item::SummonAmmo()
{
	pbody = app->physics->CreateRectangle(position.x + colliderPos.x, position.y + colliderPos.y, colliderWidth, colliderHeight, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	pbody->listener = this;

	pbody->ctype = ColliderType::AMMO;
}

bool Item::Update()
{
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 4;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 4;

	currentAnimation->Update();
	app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame());

	return true;
}

bool Item::CleanUp()
{
	app->physics->world;
	pbody->body->GetWorld()->DestroyBody(pbody->body);
	return true;
}

bool Item::LoadState(pugi::xml_node& data)
{
	position.x = data.attribute("x").as_int();
	position.y = data.attribute("y").as_int();

	active = true;
	SummonItem();

	b2Vec2 vel;
	vel.x = data.child("velocity").attribute("x").as_int();
	vel.y = data.child("velocity").attribute("y").as_int();
	pbody->body->SetLinearVelocity(vel);
	LOG("load Item");
	return true;
}

bool Item::SaveState(pugi::xml_node& data)
{
	//pugi::xml_node Enemy = data.append_child("Enemy");

	data.append_attribute("x") = position.x;
	data.append_attribute("y") = position.y;
	data.append_child("velocity");
	data.child("velocity").append_attribute("x") = pbody->body->GetLinearVelocity().x;
	data.child("velocity").append_attribute("y") = pbody->body->GetLinearVelocity().y;

	return true;
}

void Item::OnCollision(PhysBody* physA, PhysBody* physB) {

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