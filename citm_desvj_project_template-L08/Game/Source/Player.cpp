#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Window.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"


Player::Player(pugi::xml_node params) : Entity(EntityType::PLAYER)
{
	name.Create("Player");
	startPosition.x = round(params.attribute("x").as_float());
	startPosition.y = round(params.attribute("y").as_float());
	texturePath = params.attribute("texturepath").as_string();
	texturePath = "Assets/Textures/Player.png";

	SString name = params.attribute("name").as_string();

	parameters = params;
}

Player::~Player() {

}

bool Player::Awake() {

	//L02: DONE 1: Initialize Player parameters
	//L02: DONE 5: Get Player parameters from XML
	//LOG("%s", *parameters);

	position = startPosition;
	LOG(name.GetString());
	//texturePath = parameters.child("properties").child("property").attribute("texturepath").as_string();
	
	//texturePath = "Assets/Textures/Player.png";

	idle.PushBack({ 0,56,8,8 });
	idle.PushBack({ 8,56,8,8 });
	idle.PushBack({ 16,56,8,8 });
	idle.PushBack({ 24,56,8,8 });
	idleleft.PushBack({ 0,64,8,8 });
	idleleft.PushBack({ 8,64,8,8 });
	idleleft.PushBack({ 16,64,8,8 });
	idleleft.PushBack({ 24,64,8,8 });
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
	jumpright.PushBack({ 0,32,8,8 });
	jumpright.PushBack({ 8,32,8,8 });
	jumpright.PushBack({ 16,32,8,8 });
	jumpright.PushBack({ 24,32,8,8 });
	jumpleft.PushBack({ 0,40,8,8 });
	jumpleft.PushBack({ 8,40,8,8 });
	jumpleft.PushBack({ 16,40,8,8 });
	jumpleft.PushBack({ 24,40,8,8 });
	
	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	SummonPlayer();
	// L07 DONE 5: Add physics to the player - initialize physics body
	

	return true;
}

bool Player::SetPosition(int x, int y) 
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

void Player::Jump() {
	if (jump < 2 || godMode) {
		app->audio->PlayFx(1);
		pbody->body->SetLinearVelocity(b2Vec2(0, 0));
		float mass = pbody->body->GetMass();
		pbody->body->ApplyLinearImpulse(b2Vec2(0, -jumpPower * mass), pbody->body->GetWorldCenter(), true);

		jump++;
	}
}

void Player::ResetPlayer()
{	
	jump = 0;
	moveState = MS_IDLE;
	pbody->body->SetLinearVelocity(b2Vec2(0,0));

	SetPosition(startPosition.x, startPosition.y);
	LOG("load player");
}

void Player::SummonPlayer()
{
	pbody = app->physics->CreateRectangle(position.x, position.y, 8, 8, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	pbody->listener = this;

	pbody->ctype = ColliderType::PLAYER;

	moveState = MS_IDLE;

	jump = 0;
}

bool Player::Update()
{

	if (direction == 0) {
		currentAnimation = &idle;
	}
	if (direction == 1) {
		currentAnimation = &idleleft;
	}


	// L07 DONE 5: Add physics to the player - updated player position using physics	
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		moveState = MS_LEFT;
		currentAnimation = &runleft;
		direction = 1;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
		moveState = MS_IDLE;
		currentAnimation = &jumpright;
		direction = 0;

	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
		moveState = MS_IDLE;
		currentAnimation = &jumpleft;
		direction = 1;

	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		moveState = MS_RIGHT;
		currentAnimation = &runright;
		direction = 0;

	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
		currentAnimation = &jumpright;
		moveState = MS_IDLE;
		direction = 0;

	}
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		Jump();
		if (direction == 0) {
			currentAnimation = &jumpright;
		}
		if (direction == 1) {
			currentAnimation = &jumpleft;
		}

	}
	if (app->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN) {
		ResetPlayer();
	}

	Move();



	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 4;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 4;

	app->render->camera.x = -position.x * app->win->GetScale() + app->render->camera.w / 2;
	app->render->camera.y = -position.y * app->win->GetScale() + app->render->camera.h / 2 + 50;

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
