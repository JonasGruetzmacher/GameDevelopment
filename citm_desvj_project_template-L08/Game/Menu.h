/*#ifndef __MENU_H__
#define __MENU_H__

#include "EntityManager.h"
#include "Player.h"
#include "Item.h"
#include "App.h"
#include "Scene.h"


#include "Menu.h"
struct SDL_Texture;

class Menu : public Module
{
public:
	//Constructor
	Menu(bool startEnabled);

	//Destructor
	~Menu();

	// Called when the module is activated
	// Loads the necessary textures for the map background
	bool Start() override;

	// Called at the middle of the application loop
	// Updates the scene's background animations
	Update_Status Update() override;

	// Called at the end of the application loop.
	// Performs the render call of all the parts of the scene's background
	Update_Status PostUpdate() override;

	bool CleanUp() override;

public:
	// The scene sprite sheet loaded into an SDL_Texture
	SDL_Texture* bgTexture = nullptr;
	SDL_Texture* p1np2 = nullptr;
	iPoint position;


	struct characterSelected {
		bool Locked = false;
		int character = 0;
		int speed = 55;
		int x1 = 89;
		int y1 = 64;
		int x2 = 59;
		int y2 = 64;
		Particle selector;
	};

	int speed = 1;
};

#endif
*/