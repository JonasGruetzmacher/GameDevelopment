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

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateRectangle(position.x, position.y, 8,16, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	

	pbody->listener = this;

	pbody->ctype = ColliderType::PLAYER;


	pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	moveState = MS_IDLE;
	return true;
}

void Player::Jump(float jumpImpulse = 10) {
	if (jump < 2) {
		pbody->body->SetLinearVelocity(b2Vec2(0, 0));
		float mass = pbody->body->GetMass();
		pbody->body->ApplyLinearImpulse(b2Vec2(0, -jumpImpulse * mass), pbody->body->GetWorldCenter(), true);
		jump++;
	}

	
}

bool Player::Update()
{

	// L07 DONE 5: Add physics to the player - updated player position using physics	
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		moveState = MS_LEFT;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_UP) {
		moveState = MS_IDLE;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		moveState = MS_RIGHT;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_UP) {
		moveState = MS_IDLE;
	}
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN) {
		Jump();
	}
	if (app->input->GetKey(SDL_SCANCODE_U) == KEY_DOWN) {
		b2Fixture* test = (pbody->body->GetFixtureList());
		
		SString tests = SString(test->GetDensity());
		LOG(tests.GetString());

	}
	Move();


	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x)-8;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y)-8;

	app->render->DrawTexture(texture, position.x, position.y);

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
	impulseX =  (desiredVel - vel.x);
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
	}



}
