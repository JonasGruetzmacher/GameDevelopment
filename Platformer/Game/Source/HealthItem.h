#ifndef __HEALTH_H__
#define __HEALTH_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "DynArray.h"

struct SDL_Texture;

/* enum BehaviourState
{
	IDLE
};*/

class Health : public Entity
{
public:

	Health(pugi::xml_node parameters);

	virtual ~Health();

	bool Awake();

	bool Start();

	//bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

	//bool Die();

	bool SetPosition(int x, int y);

public:

private:

	//L02: DONE 1: Declare player parameters

	const char* texturePath;
	Animation* currentAnimation = nullptr;
	Animation idle;
	//Animation jumpright;
	//Animation jumpleft;
	// L07 DONE 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;
	//_moveState moveState;
	//int speed = 5;
	//bool right = true;
	//BehaviourState behaviourState = IDLE;
	SString moveClass;
	const DynArray<iPoint>* path;

	// void Move();
	//void FindPath();
	//void Jump();
	void SummonHealth();
	//void SummonFlyingEnemy();
	//void SummonWalkingEnemy();
	//void Unstuck(float posX, float posY);
};



#endif // __Coin_H__