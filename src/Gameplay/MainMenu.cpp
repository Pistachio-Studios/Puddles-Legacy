#include "Core/App.h"
#include "Core/SceneManager.h"
#include "Core/Window.h"
#include "Gameplay/MainMenu.h"

#include "Utils/Log.h"
#include "Core/GuiControl.h"
#include "Core/GuiManager.h"
#include <tracy/Tracy.hpp>


// Destructor
MainMenu::~MainMenu()
{}

// Called before the first frame
bool MainMenu::Enter()
{
	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	SDL_Rect playPos = { static_cast<int>(windowW / 2 + 200), static_cast<int>(windowH / 2 - 25), 240,50};
	playButton = (GuiControlButton*) app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Play", playPos, this);

	SDL_Rect optionsPos = { static_cast<int>(windowW / 2 + 200), static_cast<int>(windowH / 2 + 50), 240,50};
	optionsButton = (GuiControlButton*) app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "Options", optionsPos, this);

	SDL_Rect exitPos = { static_cast<int>(windowW / 2 + 200), static_cast<int>(windowH / 2 + 125), 240,50};
	exitButton = (GuiControlButton*) app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "Exit", exitPos, this);

	// Add the buttons to the buttons list
	buttons.push_back(playButton);
	buttons.push_back(optionsButton);
	buttons.push_back(exitButton);

	selectedButtonIndex = 0;
	currentButton = static_cast<GuiControlButton*>(buttons[selectedButtonIndex]);
	currentButton->isFocused = true;

	wasUpPressed = false;
    wasDownPressed = false;

	return true;
}

// Called each loop iteration
bool MainMenu::PreUpdate()
{
	// OPTICK PROFILIN
	ZoneScoped;

	return true;
}

// Called each loop iteration
bool MainMenu::Update(float dt)
{

	// Check for gamepad inputs
    if(app->input->pads[0].a == KEY_DOWN) {
        // Simulate a click on the currently selected button
        OnGuiMouseClickEvent(currentButton);
    }
    
	if(app->input->pads[0].up == KEY_DOWN) {
        if(!wasUpPressed) {
            // Move the selection up
            SelectPreviousButton();
        }
        wasUpPressed = true;
    } else {
        wasUpPressed = false;
    }

    if(app->input->pads[0].down == KEY_DOWN) {
        if(!wasDownPressed) {
            // Move the selection down
            SelectNextButton();
        }
        wasDownPressed = true;
    } else {
        wasDownPressed = false;
    }

	// OPTICK PROFILIN
	ZoneScoped;

	return true;
}

// Called each loop iteration
bool MainMenu::PostUpdate()
{
	// OPTICK PROFILIN
	ZoneScoped;

	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if(exitPressed)
		ret = false;

	return ret;
}

bool MainMenu::Exit()
{
	app->guiManager->RemoveGuiControl(playButton);
	app->guiManager->RemoveGuiControl(optionsButton);
	app->guiManager->RemoveGuiControl(exitButton);

	// TODO fix bug where if you clean the buttons, the pause menu doesn't change the scene to main menu
	// buttons.clear();

	return true;
}

// Called before quitting
bool MainMenu::CleanUp()
{
	LOG("Freeing mainmenu");
	app->guiManager->RemoveGuiControl(playButton);
	app->guiManager->RemoveGuiControl(optionsButton);
	app->guiManager->RemoveGuiControl(exitButton);
	buttons.clear();
	return true;
}

bool MainMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	switch (control->id)
	{
	case 1:
		app->sceneManager->ChangeScene("testscene");
		break;
	case 2:
		break;
	case 3:
		exitPressed = true;
		break;
	}
	
	return true;
}

void MainMenu::SelectPreviousButton()
{
	currentButton->isFocused = false;
    // Move the selection up
    selectedButtonIndex--;
    if(selectedButtonIndex < 0)
    {
        selectedButtonIndex = buttons.size() - 1; // Wrap around to the last button
    }

    // Select the new button
	currentButton = static_cast<GuiControlButton*>(buttons[selectedButtonIndex]);
	currentButton->isFocused = true;

	// Debug
	LOG("Selected button: %d", selectedButtonIndex);
}

void MainMenu::SelectNextButton()
{
	currentButton->isFocused = false;
	// Move the selection down
	selectedButtonIndex++;
	if(selectedButtonIndex >= buttons.size())
	{
		selectedButtonIndex = 0; // Wrap around to the first button
	}

	// Select the new button
	currentButton = static_cast<GuiControlButton*>(buttons[selectedButtonIndex]);

	currentButton->isFocused = true;

	// Debug
	LOG("Selected button: %d", selectedButtonIndex);
}

void MainMenu::RenderGUI()
{
	
}
