#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "Module.h"
#include "GuiButton.h"
#include "GuiSlider.h"

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

private:
	SDL_Rect logoRect;
	SDL_Texture* logo;
	const char* texturePath;

	GuiButton* playButton;
	GuiButton* continueButton;
	GuiButton* settingsButton;

	GuiSlider* musicSlider;
	GuiSlider* fxSlider;

};

#endif // __TITLE_SCENE_H__
