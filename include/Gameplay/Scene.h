#ifndef __SCENE_H__
#define __SCENE_H__

#include "Utils/SString.h"

#include <pugixml.hpp>


class App;
class GuiControl;

class Scene
{
public:

	Scene() : active(false)
	{}

	Scene(SString sceneName) : name(sceneName) {}

	// Called before the first frame
	virtual bool Enter()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	virtual bool Exit()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual void DrawImGui()
	{
	}

	// Called when we want to load data from XML
	virtual bool LoadState(pugi::xml_node node)
	{
		return true;
	}

	// Called when we want to save data from XML
	virtual bool SaveState(pugi::xml_node node)
	{
		return true;
	}

	virtual bool OnGuiMouseClickEvent(GuiControl* control)
	{
		return true;
	}

public:

	SString name;
	bool active;
	pugi::xml_node parameters;
};

#endif // __Scene_H__