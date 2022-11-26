#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

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

	bool Die();

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
	// L07 DONE 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;
	//_moveState moveState;
	int speed = 15;
	int direction = 0;


	void Move();

	void Jump();
	void ResetEnemy();
	void SummonEnemy();

};



#endif // __Enemy_H__
