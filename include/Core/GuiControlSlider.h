#ifndef __GUICONTROLSLIDER_H__
#define __GUICONTROLSLIDER_H__

#include "Core/GuiControl.h"

#include "Utils/Point.h"
#include "Utils/SString.h"

class GuiControlSlider : public GuiControl
{

public:

	GuiControlSlider(uint32 id, const char* text, SDL_Rect bounds, int minValue, int maxValue);
	virtual ~GuiControlSlider();

	// Called each loop iteration
	bool Update(float dt);
	bool Draw();

public:
	bool isPressed = false;
	int currentValue;
	int currentPos;

private:

	SDL_Rect rect2;
	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;


	int minValue;
	int maxValue;

	iPoint initialPos;
};

#endif // __GUICONTROLBUTTON_H__