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

Bullet::Bullet(iPoint spawnPosition, bool rightDirection) : Entity(EntityType::BULLET)
{
	name.Create("bullet");
	position = spawnPosition;
}

Bullet::~Bullet() {}

bool Bullet::Awake() {

	return true;
}

bool Bullet::Start() {

	//initilize textures
	//texture = app->tex->Load(texturePath);
	
	// L07 DONE 4: Add a physics to an Bullet - initialize the physics body
	pbody = app->physics->CreateCircle(position.x, position.y, 1, bodyType::DYNAMIC);

	pbody->ctype = ColliderType::BULLET;


	return true;
}

bool Bullet::Update()
{
	// L07 DONE 4: Add a physics to an Bullet - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	//app->render->DrawTexture(texture, position.x, position.y);

	return true;
}

bool Bullet::CleanUp()
{
	return true;
}