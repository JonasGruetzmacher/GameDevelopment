#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "DynArray.h"

struct SDL_Texture;

class Item : public Entity
{
public:

	Item(pugi::xml_node parameters);

	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

	void SummonItem();
	void SummonCoin();
	void SummonHeart();
	void SummonAmmo();

	bool SetPosition(int x, int y);

public:

	bool isPicked = false;

	SString itemClass;

private:

	const char* texturePath;

	Animation* currentAnimation = nullptr;
	Animation idle;

	//DONE 4: Add a physics to an item
	PhysBody* pbody;
};

#endif // __ITEM_H__