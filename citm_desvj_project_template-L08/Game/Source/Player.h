#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;
enum _moveState {
	MS_LEFT,
	MS_RIGHT,
	MS_IDLE,
	MS_JUMP
};
class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

	bool Die();

	bool godMode = false;

public:

private:

	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	// L07 DONE 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;
	_moveState moveState;
	int jump = 0;
	int pickCoinFxId;
	int speed = 8;

	

	void Move();

	void Jump(float impulse);
	
};



#endif // __PLAYER_H__