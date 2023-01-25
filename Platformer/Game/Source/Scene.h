#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"

struct Level {
	int id;
	SString name;
};



struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	bool SetUp(int level);

	void SetGui(bool guiOn);

private:
	bool Pause();
	bool Resume();

	bool DrawGameUI();

public:

	

	//L02: DONE 3: Declare a Player attribute 
	Player* player;
	int currentLevel;
	bool restartLevel = false;
	bool nextLevel = false;
	bool debugMode = false;
	bool showPauseMenu = false;

private:
	SDL_Texture* img;

	const char* mouseTilePath;
	SDL_Texture* mouseTileTex = nullptr;
	const char* originTilePath;
	SDL_Texture* originTex = nullptr;
	const char* settingsBackgroundPath;
	SDL_Texture* settingsBackground;

	const char* gameUIPath;
	SDL_Texture* gameUI = nullptr;
	SDL_Rect uiHeart;
	SDL_Rect uiCoin;
	SDL_Rect uiAmmo;
	SDL_Rect uiBackground;



	List<Level*> levels;

	//Debug
	iPoint origin;
	bool originSelected = false;

	GuiButton* settingsButton;
	GuiButton* resumeButton;
	GuiButton* quitButton;
	GuiButton* titleScreenButton;

	GuiSlider* musicSlider;
	GuiSlider* fxSlider;

	GuiCheckBox* fullscreenCheckBox;
	GuiCheckBox* vSyncCheckBox;

	bool showSettings = false;
	

	bool quit = false;
};

#endif // __SCENE_H__