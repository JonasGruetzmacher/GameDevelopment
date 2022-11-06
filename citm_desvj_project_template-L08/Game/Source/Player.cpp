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
	//pos = position;
	//texturePath = "Assets/Textures/player/idle1.png";

	//L02: DONE 5: Get Player parameters from XML
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	idle.PushBack({ 0,56,8,8 });
	idle.PushBack({ 8,56,8,8 });
	idle.PushBack({ 16,57,8,8 });
	idle.PushBack({ 24,57,8,8 });
	//idleleft.Pushback({ 0,66,6,72 });
	//idleleft.Pushback({ 8,66,14,72 });
	//idleleft.Pushback({ 16,67,22,72 });
	//idleleft.Pushback({ 24,67,6,72 });
	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateRectangle(position.x, position.y, 8, 16, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);


	pbody->listener = this;

	pbody->ctype = ColliderType::PLAYER;

	moveState = MS_IDLE;

	return true;
}

void Player::Jump(float jumpImpulse = 15) {
	if (jump < 2 || godMode) {
		app->audio->PlayFx(1);
		pbody->body->SetLinearVelocity(b2Vec2(0, 0));
		float mass = pbody->body->GetMass();
		pbody->body->ApplyLinearImpulse(b2Vec2(0, -jumpImpulse * mass), pbody->body->GetWorldCenter(), true);

		jump++;
	}


}

bool Player::Update()
{
	currentAnimation = &idle;
	// L07 DONE 5: Add physics to the player - updated player position using physics	
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		moveState = MS_LEFT;
		currentAnimation = &idle;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
		moveState = MS_IDLE;
		currentAnimation = &idle;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
		moveState = MS_IDLE;
		currentAnimation = &idle;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		moveState = MS_RIGHT;
		currentAnimation = &idle;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
		currentAnimation = &idle;
		moveState = MS_IDLE;
	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		Jump();
		currentAnimation = &idle;
	}
	if (app->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN) {
		b2Fixture* test = (pbody->body->GetFixtureList());

		SString tests = SString(test->GetDensity());
		LOG(tests.GetString());
	}

	Move();



	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 8;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 8;

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
	app->LoadGameRequest();

	return true;
}

bool Player::LoadState(pugi::xml_node& data)
{
	b2Vec2 transform;
	transform.x = data.attribute("x").as_int() + 0.2;
	transform.y = data.attribute("y").as_int() + 1;
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
