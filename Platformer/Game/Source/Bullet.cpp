#include "Bullet.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "EntityManager.h"

Bullet::Bullet(iPoint spawnPosition, bool rightDirection) : Entity(EntityType::BULLET)
{
	name.Create("bullet");
	position = spawnPosition;
	lookDirection = rightDirection;
}

Bullet::~Bullet() {}

bool Bullet::Awake() {

	return true;
}

bool Bullet::Start() {

	//initilize textures
	//texture = app->tex->Load(texturePath);
	
	// L07 DONE 4: Add a physics to an Bullet - initialize the physics body
	if(lookDirection) pbody = app->physics->CreateRectangle(position.x + 10, position.y + 3,2, 2, bodyType::DYNAMIC);
	else pbody = app->physics->CreateRectangle(position.x - 1, position.y + 3, 2, 2, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	pbody->body->SetBullet(true);
	pbody->body->SetGravityScale(0);
	pbody->listener = this;
	pbody->ctype = ColliderType::BULLET;


	return true;
}

bool Bullet::Update()
{
	// L07 DONE 4: Add a physics to an Bullet - update the position of the object from the physics. 
	Move();
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);

	//app->render->DrawTexture(texture, position.x, position.y);
	app->render->DrawRectangle({position.x, position.y, 2,2},200U,0,0);

	return true;
}

void Bullet::Move() {
	if(lookDirection) pbody->body->SetLinearVelocity(b2Vec2(ceil(speed * app->dt * 0.1),0));
	else pbody->body->SetLinearVelocity(b2Vec2(-ceil(speed * app->dt * 0.1), 0));

}

bool Bullet::CleanUp()
{
	pbody->body->GetWorld()->DestroyBody(pbody->body);
	delete pbody;
	return true;
}

void Bullet::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		app->entityManager->DestroyEntity(physA->listener);
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		app->entityManager->DestroyEntity(physA->listener);
		break;
	case ColliderType::WALL:
		LOG("Collision WALL");
		app->entityManager->DestroyEntity(physA->listener);
		break;
	case ColliderType::WATER:
		LOG("Collision WATER");
		break;
	case ColliderType::ENEMY:
		LOG("Collision Enemy");
		app->entityManager->DestroyEntity(physB->listener);
		app->scene->player->score += 100;
	case ColliderType::BULLET:
		app->entityManager->DestroyEntity(physA->listener);

		break;
	}
}