#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Core/Module.h"
#include "Core/GuiControl.h"

#include "Utils/List.h"

class GuiManager : public Module
{
public:

	// Constructor
	GuiManager();

	GuiManager(bool startEnabled);

	// Destructor
	virtual ~GuiManager();

	// Called before the first frame
	 bool Start();

	 // Called each loop iteration
	 bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds = { 0,0,0,0 });

	void RemoveGuiControl(GuiControl* entity);

public:

	List<GuiControl*> guiControlsList;
	SDL_Texture* texture;

};

#endif // __GUIMANAGER_H__