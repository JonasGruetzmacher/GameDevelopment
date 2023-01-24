#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"
#include "Timer.h"

class GuiCheckBox : public GuiControl
{
public:

	GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text, int texId);
	virtual ~GuiCheckBox();

	bool Update(float dt);
	bool Draw(Render* render);

	void SetValue(bool toggle);
	bool GetValue();

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;

	SDL_Texture* buttonTex;
	SDL_Texture* checkBoxTex;

	int texId;

	GuiControlState preState;

	uint click1FxId;
	uint click2FxId;

	int boundx;

	GuiControlState previousState;

	int height;
	int width;

	bool toggle;

};

#endif // __GUICHECKBOX_H__