#ifndef __GUICONTROLCHECKBOX_H__
#define __GUICONTROLCHECKBOX_H__

#include "Core/GuiControl.h"
#include "Utils/Point.h"
#include "Utils/SString.h"
#include "Utils/Timer.h"

class GuiControlCheckbox : public GuiControl
{
public:
	GuiControlCheckbox(uint32 id, const char* text, SDL_Rect bounds);
	virtual ~GuiControlCheckbox();

	// Called each loop iteration
	bool Update(float dt);
	bool Draw();

public:
	bool isPressed = false;
	bool checked = false;

	Timer controlEaseTimer;

private:
	SDL_Rect rect2;
	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;

	iPoint initialPos;
};

#endif // __GUICONTROLCHECKBOX_H__