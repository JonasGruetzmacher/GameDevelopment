

#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

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

	Player(pugi::xml_node parameters);

	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

	bool Die();

	bool SetPosition(int x, int y);

	bool godMode = false;

public:
	int direction = 0;
private:

	//L02: DONE 1: Declare player parameters
	
	const char* texturePath;
	Animation* currentAnimation = nullptr;
	Animation idle;
	Animation idleleft;
	Animation runright;
	Animation runleft;
	Animation jumpright;
	Animation jumpleft;
	// L07 DONE 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;
	_moveState moveState;
	int jump = 0;
	int pickCoinFxId;
	int speed = 15;
	int jumpPower = 30;
	


	void Move();

	void Jump();
	void ResetPlayer();
	void SummonPlayer();
	void Shoot();

};



#endif // __PLAYER_H__
