#include "Core/GuiManager.h"
#include "Core/App.h"
#include "Core/GuiControl.h"
#include "Utils/List.h"
#include "Core/Textures.h"

#include "Core/GuiControlButton.h"
#include "Core/GuiControlLabel.h"
#include "Core/GuiControlPopUp.h"
#include "Core/GuiControlSlider.h"
#include "Core/GuiControlCheckbox.h"
#include "Core/Audio.h"
#include <cstddef>

GuiManager::GuiManager() :Module()
{
	name.Create("guiManager");
}

GuiManager::GuiManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	return true;
}

// L15: DONE1: Implement CreateGuiControl function that instantiates a new GUI control and add it to the list of controls
GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Scene* observer, int minV, int maxV, SDL_Rect sliderBounds)
{
	GuiControl* guiControl = nullptr;

	//Call the constructor according to the GuiControlType
	switch (type)
	{
	case GuiControlType::BUTTON:
		guiControl = new GuiControlButton(id, bounds, text);
		break;
	case GuiControlType::LABEL:
		guiControl = new GuiControlLabel(id, bounds, text);
		break;
	case GuiControlType::POPUP:
		guiControl = new GuiControlPopUp(id);
		break;
	case GuiControlType::SLIDER:
		guiControl = new GuiControlSlider(id, text, bounds, minV, maxV);
		break;
	case GuiControlType::CHECKBOX:
		guiControl = new GuiControlCheckbox(id, text, bounds);
		break;
	}
	
	//Set the observer
	guiControl->observer = observer;

	// Created GuiControls are add it to the list of controls
	guiControlsList.Add(guiControl);

	return guiControl;
}

void GuiManager::RemoveGuiControl(GuiControl* entity)
{
/* 	ListItem<GuiControl*>* control = guiControlsList.start;
	ListItem<GuiControl*>* nextControl = nullptr;

	while (control != nullptr)
	{
		nextControl = control->next;

		if (control->data == entity)
		{
			guiControlsList.Del(control);
			control = nullptr;
			break;
		}
		control = nextControl;
	} */

	controlsToDelete.PushBack(entity);

}

bool GuiManager::Update(float dt)
{	
	//Delete the controls that are marked to be deleted
	for (uint i = 0; i < controlsToDelete.Count(); i++)
	{
		GuiControl* controlData = controlsToDelete[i];
		int index = guiControlsList.Find(controlData);
		ListItem<GuiControl*>* controlItem = guiControlsList.At(index);
		guiControlsList.Del(controlItem);

		//BORRAR!!!!!!!!
		//guiControlsList.Del(guiControlsList.At(guiControlsList.Find(controlsToDelete[i])));
	}

	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		control->data->Update(dt);
		control = control->next;
	}

	return true;
}

bool GuiManager::CleanUp()
{
	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		RELEASE(control);
	}

	return true;

	return false;
}



