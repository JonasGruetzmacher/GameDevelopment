#ifndef __BULLET_H__
#define __BULLET_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class Bullet : public Entity
{
public:

	Bullet(iPoint position, bool rightDirection);
	virtual ~Bullet();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

public:

	bool isPicked = false;

private:

	SDL_Texture* texture;
	const char* texturePath;

	//DONE 4: Add a physics to an item
	PhysBody* pbody;
};

#endif // __BULLET_H__