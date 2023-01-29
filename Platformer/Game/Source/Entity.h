#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"
#include "Map.h"
#include "App.h"

class PhysBody;

enum class EntityType
{
	PLAYER,
	ITEM,
	ENEMY,
	COIN,
	BULLET,
	UNKNOWN
};

class Entity
{
public:

	Entity(EntityType type) : type(type), active(true) {}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update()
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&)
	{
		return true;
	}

	void Entity::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Entity::Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	virtual void OnCollision(PhysBody* physA, PhysBody* physB) {
	
	}

	void Entity::GetTextureWithGid() {
		TileSet* tileset = app->map->GetTilesetFromTileId(gid);
		texture = tileset->texture;
	}

	TileSet* Entity::GetTileSetWithGid() {
		return (app->map->GetTilesetFromTileId(gid));
	};

public:

	SString name;
	EntityType type;
	bool active = true;
	pugi::xml_node parameters;

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	Tile* tile;
	fPoint colliderPos;
	float colliderWidth;
	float colliderHeight;
	bool lookDirection;
	iPoint position;
	iPoint startPosition;
	uint gid;
	SDL_Texture* texture;
	bool renderable = true;
	bool toDestroy;
};

#endif // __ENTITY_H__