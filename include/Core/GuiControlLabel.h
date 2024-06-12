#ifndef __GUICONTROLLABEL_H__
#define __GUICONTROLLABEL_H__

#include "Core/GuiControl.h"

#include "Utils/Point.h"
#include "Utils/SString.h"
#include "Utils/Timer.h"

class GuiControlLabel : public GuiControl
{

public:

	GuiControlLabel(uint32_t id, SDL_Rect bounds, const char* text);
	virtual ~GuiControlLabel();

	// Called each loop iteration
	bool Update(float dt);

	//easings
	Timer controlEaseTimer;

private:

	bool drawBasic = false;
};

#endif // __GUICONTROLLABEL_H__