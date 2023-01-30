#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "DynArray.h"

struct SDL_Texture;

enum BehaviourState 
{
	IDLE,
	CHASE
};

class Enemy : public Entity
{
public:

	Enemy(pugi::xml_node parameters);

	virtual ~Enemy();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

	bool TakeDamage(int amount);

	bool SetPosition(int x, int y);

public:

private:

	//L02: DONE 1: Declare player parameters

	const char* texturePath;
	Animation* currentAnimation = nullptr;
	Animation idle;
	Animation idleleft;
	Animation runright;
	Animation runleft;
	//Animation jumpright;
	//Animation jumpleft;
	// L07 DONE 5: Add physics to the enemy - declare a Physics body
	PhysBody* pbody;
	//_moveState moveState;
	int speed = 5;
	bool right = true;
	BehaviourState behaviourState = IDLE;
	SString moveClass;
	bool isJumping = false;
	const DynArray<iPoint>* path;

	void Move();
	void FindPath();
	void Jump();
	void SummonEnemy();
	void SummonFlyingEnemy();
	void SummonWalkingEnemy();
	void Unstuck(float posX, float posY);
	void Die();

	int health = 2;
};



#endif // __Enemy_H__
