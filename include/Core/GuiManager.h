#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Core/Module.h"
#include "Core/GuiControl.h"

#include "Utils/DynArray.h"
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

    int GetGuiLastId() const;

    // Additional methods
	GuiControl* CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Scene* observer, SDL_Texture* texture = nullptr, int minV = 0, int maxV = 0, SDL_Rect sliderBounds = { 0,0,0,0 });

	void RemoveGuiControl(GuiControl* entity);

public:

	List<GuiControl*> guiControlsList;
	SDL_Texture* texture;

private:
	DynArray<GuiControl*> controlsToDelete;
};

#endif // __GUIMANAGER_H__
