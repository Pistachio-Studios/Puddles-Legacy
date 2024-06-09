#ifndef __GUICONTROL_H__
#define __GUICONTROL_H__

#include "Core/Module.h"

#include "Gameplay/Scene.h"
#include "Utils/SString.h"
#include <variant>

#include <SDL.h>

enum class GuiControlType
{
	BUTTON,
	LABEL,
	TOGGLE,
	CHECKBOX,
	SLIDER,
	SLIDERBAR,
	COMBOBOX,
	DROPDOWNBOX,
	INPUTBOX,
	VALUEBOX,
	SPINNER,
	POPUP
};

enum class GuiControlState
{
	DISABLED,
	NORMAL,
	FOCUSED,
	PRESSED,
	SELECTED
};

class GuiControl
{
public:

	// Constructor
	GuiControl(GuiControlType type, uint32_t id) : type(type), id(id), state(GuiControlState::NORMAL) {}

	// Constructor
	GuiControl(GuiControlType type, SDL_Rect bounds, const char* text) :
		type(type),
		state(GuiControlState::NORMAL),
		bounds(bounds),
		text(text)
	{
		color.r = 255; color.g = 255; color.b = 255;
		texture = NULL;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// 
	void SetTexture(SDL_Texture* tex)
	{
		texture = tex;
		section = { 0, 0, 0, 0 };
	}

	// 
	void SetObserver(Scene* scene)
	{
		observer = scene;
	}

	// 
	void SetObserver(Module* module)
	{
		observer = module;
	}

	// 
	void NotifyObserver()
	{
		if (std::holds_alternative<Scene*>(observer))
		{
			Scene* scene = std::get<Scene*>(observer);
			scene->OnGuiMouseClickEvent(this);
		}
		else if (std::holds_alternative<Module*>(observer))
		{
			Module* module = std::get<Module*>(observer);
			module->OnGuiMouseClickEvent(this);
		}
	}

public:

	uint32_t id;
	GuiControlType type;
	GuiControlState state;

	SString text;           // Control text (if required)
	SDL_Rect bounds;        // Position and size
	SDL_Color color;        // Tint color

	SDL_Texture* texture = nullptr;   // Texture atlas reference
	SDL_Texture* textureSelected = nullptr;   // Texture atlas reference
	SDL_Texture* textureSelectedLeft = nullptr; 
	SDL_Texture* textureSelectedRight = nullptr;
	SDL_Rect section;       // Texture atlas base section

	//Scene* observer;        // Observer 
	std::variant<Scene*, Module*> observer; // Observer

	bool markedToDelete = false;
};

#endif // __GUICONTROL_H__