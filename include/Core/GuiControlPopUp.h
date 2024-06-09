#ifndef __GUICONTROLPOPUP_H__
#define __GUICONTROLPOPUP_H__

#include "Core/GuiControl.h"

#include "Utils/Point.h"
#include "Utils/SString.h"
#include "Core/Textures.h"

class GuiControlPopUp : public GuiControl
{

public:

	GuiControlPopUp(uint32_t id, SDL_Rect bounds, SDL_Texture* texture);
	virtual ~GuiControlPopUp();

	// Called each loop iteration
	bool Update(float dt);

private:

	int posX, posY;
	SDL_Texture* texture1;
	uint windowW, windowH;

};

#endif // __GUICONTROLPOPUP_H__