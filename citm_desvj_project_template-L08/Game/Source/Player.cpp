#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"


Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	//L02: DONE 1: Initialize Player parameters

	//L02: DONE 5: Get Player parameters from XML
	position.x = round(parameters.attribute("x").as_float());
	position.y = round(parameters.attribute("y").as_int());
	startPosition = position;
	SString name = parameters.attribute("name").as_string();
	LOG(name.GetString());
	//texturePath = parameters.child("properties").child("property").attribute("texturepath").as_string();
	texturePath = parameters.attribute("texturepath").as_string();

	idle.PushBack({ 0,56,8,8 });
	idle.PushBack({ 8,56,8,8 });
	idle.PushBack({ 16,56,8,8 });
	idle.PushBack({ 24,56,8,8 });
	idleleft.PushBack({ 0,64,8,8 });
	idleleft.PushBack({ 8,64,8,8 });
	idleleft.PushBack({ 16,65,8,8 });
	idleleft.PushBack({ 24,65,8,8 });
	runright.PushBack({ 0,8,8,8 });
	runright.PushBack({ 8,8,8,8 });
	runright.PushBack({ 16,8,8,8 });
	runright.PushBack({ 24,8,8,8 });
	runright.PushBack({ 32,8,8,8 });
	runright.PushBack({ 40,8,8,8 });
	runright.PushBack({ 48,8,8,8 });
	runright.PushBack({ 56,8,8,8 });
	runleft.PushBack({ 0,16,8,8 });
	runleft.PushBack({ 8,16,8,8 });
	runleft.PushBack({ 16,16,8,8 });
	runleft.PushBack({ 24,16,8,8 });
	runleft.PushBack({ 32,16,8,8 });
	runleft.PushBack({ 40,16,8,8 });
	runleft.PushBack({ 48,16,8,8 });
	runleft.PushBack({ 56,16,8,8 });
	
	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateRectangle(position.x, position.y, 8, 8, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);


	pbody->listener = this;

	pbody->ctype = ColliderType::PLAYER;

	moveState = MS_IDLE;

	return true;
}

bool Player::SetPosition(int x, int y) 
{
	bool ret = true;

	LOG("TESTEST");


	return ret;
}

void Player::Jump() {
	if (jump < 2 || godMode) {
		app->audio->PlayFx(1);
		pbody->body->SetLinearVelocity(b2Vec2(0, 0));
		float mass = pbody->body->GetMass();
		pbody->body->ApplyLinearImpulse(b2Vec2(0, -jumpPower * mass), pbody->body->GetWorldCenter(), true);

		jump++;
	}
}

bool Player::Update()
{


	currentAnimation = &idle;


	// L07 DONE 5: Add physics to the player - updated player position using physics	
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		moveState = MS_LEFT;
		currentAnimation = &runleft;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
		moveState = MS_IDLE;
		currentAnimation = &runright;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
		moveState = MS_IDLE;
		currentAnimation = &runleft;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		moveState = MS_RIGHT;
		currentAnimation = &runright;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
		currentAnimation = &runright;
		moveState = MS_IDLE;
	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		Jump();
		currentAnimation = &idle;
	}
	if (app->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN) {
		SetPosition(150,320);
	}

	Move();



	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 4;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 4;

	app->render->camera.x = -position.x * 1.5 - 100;
	app->render->camera.y = -position.y * 3 + 350;

	currentAnimation->Update();
	app->render->DrawTexture(texture, position.x, position.y, &currentAnimation->GetCurrentFrame());
	//currentAnimation->Update();
	return true;
}

bool Player::CleanUp()
{
	return true;
}

void Player::Move() {
	b2Vec2 vel = pbody->body->GetLinearVelocity();
	float desiredVel = 0;
	float impulseX = 0;
	float impulseY = 0;
	switch (moveState)
	{
	case MS_LEFT:
		desiredVel = -speed;
		break;
	case MS_RIGHT:
		desiredVel = speed;
		break;
	case MS_IDLE:
		desiredVel = 0;
		break;
	default:
		break;
	}
	impulseX = (desiredVel - vel.x);
	float mass = pbody->body->GetMass();
	pbody->body->ApplyLinearImpulse(b2Vec2(impulseX, impulseY), pbody->body->GetWorldCenter(), true);

}
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	// L07 DONE 7: Detect the type of collision

	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		jump = 0;
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		jump = 0;
		break;
	case ColliderType::WALL:
		LOG("Collision WALL");
		break;
	case ColliderType::WATER:
		LOG("Collision WATER");
		if (!godMode) {
			Die();
		}

		break;
	}
}

bool Player::Die() {
	LOG("Player died");
	app->audio->PlayFx(2);
	//SetPosition(startPosition.x, startPosition.y);
	app->LoadGameRequest();

	return true;
}

bool Player::LoadState(pugi::xml_node& data)
{
	b2Vec2 transform;
	transform.x = data.attribute("x").as_int();
	transform.y = data.attribute("y").as_int();
	pbody->body->SetTransform(transform, 0);
	position.x = METERS_TO_PIXELS(transform.x);
	position.y = METERS_TO_PIXELS(transform.y);

	jump = data.attribute("jump").as_int();

	b2Vec2 vel;
	vel.x = data.child("velocity").attribute("x").as_int();
	vel.y = data.child("velocity").attribute("y").as_int();
	pbody->body->SetLinearVelocity(vel);
	LOG("load player");
	return true;
}

// L03: DONE 8: Create a method to save the state of the renderer
// using append_child and append_attribute
bool Player::SaveState(pugi::xml_node& data)
{
	//pugi::xml_node player = data.append_child("player");

	data.append_attribute("x") = pbody->body->GetTransform().p.x;
	data.append_attribute("y") = pbody->body->GetTransform().p.y;
	data.append_attribute("jump") = jump;
	data.append_child("velocity");
	data.child("velocity").append_attribute("x") = pbody->body->GetLinearVelocity().x;
	data.child("velocity").append_attribute("y") = pbody->body->GetLinearVelocity().y;

	return true;
}
