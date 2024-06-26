#ifndef __GUICONTROLBUTTON_H__
#define __GUICONTROLBUTTON_H__

#include "Core/GuiControl.h"

#include "Utils/Point.h"
#include "Utils/SString.h"
#include "Utils/Timer.h"

class GuiControlButton : public GuiControl
{

public:

	GuiControlButton(uint32_t id, SDL_Rect bounds, const char* text);
	virtual ~GuiControlButton();

	// Called each loop iteration
	bool Update(float dt);

	//easings
	Timer controlEaseTimer;

private:

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;

	SDL_Color textColor = { 255, 255, 255, 255 };
};

#endif // __GUICONTROLBUTTON_H__