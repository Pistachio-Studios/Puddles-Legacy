#ifndef __GUICONTROLDROPDOWNBOX_H__
#define __GUICONTROLDROPDOWNBOX_H__

#include "Core/GuiControl.h"
#include "Utils/Point.h"
#include "Utils/SString.h"
#include "Core/GuiControlButton.h"
#include <list>

class GuiControlDropDownBox : public GuiControl
{
public:
	GuiControlDropDownBox(uint32 id, const char* text, SDL_Rect bounds);
	virtual ~GuiControlDropDownBox();

	// Called each loop iteration
	bool Update(float dt);
	bool Draw();
	GuiControlButton* AddOption(const std::string& optionText, Scene* observer);

    int GetSelectedOption() const;

public:
	bool isPressed = false;
	bool isOpen = false; // Changed from 'checked' to 'isOpen'
	//easings
	Timer controlEaseTimer;

private:
	std::list<GuiControlButton*> options;
	SDL_Rect rect2;
	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;

	iPoint initialPos;
};

#endif // __GUICONTROLDROPDOWNBOX_H__