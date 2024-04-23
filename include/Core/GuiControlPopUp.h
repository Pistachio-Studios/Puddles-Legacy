#ifndef __GUICONTROLPOPUP_H__
#define __GUICONTROLPOPUP_H__

#include "Core/GuiControl.h"

#include "Utils/Point.h"
#include "Utils/SString.h"

class GuiControlPopUp : public GuiControl
{

public:

	GuiControlPopUp(uint32_t id);
	virtual ~GuiControlPopUp();

	// Called each loop iteration
	bool Update(float dt);

private:

	int mouseX, mouseY;
	unsigned int click;
	uint windowW, windowH;
	bool canClick = true;
	bool drawBasic = false;
};

#endif // __GUICONTROLPOPUP_H__