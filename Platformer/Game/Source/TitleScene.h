#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "Module.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiToggle.h"
#include "GuiCheckBox.h"

struct SDL_Texture;

class TitleScene : public Module
{
public:

	TitleScene();

	// Destructor
	virtual ~TitleScene();

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

public:
	bool showSettings = false;

private:
	SDL_Rect logoRect;
	SDL_Texture* logo;
	SDL_Texture* settingsBackground;
	SDL_Texture* creditsTex;
	const char* texturePath;

	GuiButton* playButton;
	GuiButton* continueButton;
	GuiButton* settingsButton;
	GuiButton* quitButton;
	GuiButton* creditsButton;

	GuiSlider* musicSlider;
	GuiSlider* fxSlider;

	GuiCheckBox* fullscreenCheckBox;
	GuiCheckBox* vSyncCheckBox;

	bool quit = false;
	bool showCredits;

};

#endif // __TITLE_SCENE_H__
