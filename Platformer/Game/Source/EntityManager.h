#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "List.h"

class EntityManager : public Module
{
public:

	EntityManager();

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type, pugi::xml_node parameters);

	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);
	void CreateBullet(Entity* shootingEntity);

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

public:

	List<Entity*> entities;

};

#endif // __ENTITYMANAGER_H__