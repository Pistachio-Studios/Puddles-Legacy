#include "Core/App.h"
#include "Core/SceneManager.h"
#include "Core/Window.h"
#include "Core/Audio.h"
#include "Gameplay/MainMenu.h"
#include "Core/Textures.h"
#include "Core/Render.h"

#include "Utils/Log.h"
#include "Core/GuiControl.h"
#include "Core/GuiControlButton.h"
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

	SDL_Rect playPos = { static_cast<int>(windowW / 2 - 170), static_cast<int>(windowH / 2 + 50), 340,75};
	playButton = (GuiControlButton*) app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "  Play  ", playPos, this);

	SDL_Rect optionsPos = { static_cast<int>(windowW / 2 - 170), static_cast<int>(windowH / 2 + 150), 340,75};
	optionsButton = (GuiControlButton*) app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, " Options ", optionsPos, this);

	SDL_Rect exitPos = { static_cast<int>(windowW / 2 - 170), static_cast<int>(windowH / 2 + 250), 340,75};
	exitButton = (GuiControlButton*) app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "  Exit  ", exitPos, this);

	gameTitle = app->tex->Load(parameters.child("gameTitle").attribute("texturepath").as_string());

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
	// OPTICK PROFILIN
	ZoneScoped;

	app->render->DrawTexture(gameTitle, 0, 0);

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
	return true;
}

// Called before quitting
bool MainMenu::CleanUp()
{
	LOG("Freeing MainMenu");
	app->tex->UnLoad(gameTitle);
	app->guiManager->RemoveGuiControl(playButton);
	app->guiManager->RemoveGuiControl(optionsButton);
	app->guiManager->RemoveGuiControl(exitButton);
	return true;
}

bool MainMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	switch (control->id)
	{
	case 1:
		app->sceneManager->ChangeScene("tutorialscene");
		app->guiManager->RemoveGuiControl(crossOButton);
		crossOButton = nullptr;
		app->guiManager->RemoveGuiControl(music);
		music = nullptr;
		break;
	case 2:
		if (popUpOptions == nullptr) {
			popUpOptions = (GuiControlPopUp*)app->guiManager->CreateGuiControl(GuiControlType::POPUP, 4, "", { 0,0,0,0 }, this);
			SDL_Rect crossOButtonPos = { static_cast<int>(windowW / 2 + 100), static_cast<int>(windowH / 2 - 25), 30, 30 };
			crossOButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "X", crossOButtonPos, this);
			SDL_Rect musicPos = { static_cast<int>(windowW / 2 - 90), static_cast<int>(windowH / 2 + 30), 150, 20 };
			music = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 6, "Music ", musicPos, this, 0, 128);
		}
		break;
	case 3:
		exitPressed = true;
		break;
	case 5:
		if (crossOButton != nullptr) {
				app->guiManager->RemoveGuiControl(popUpOptions);
				popUpOptions = nullptr;
				app->guiManager->RemoveGuiControl(crossOButton);
				crossOButton = nullptr;
				app->guiManager->RemoveGuiControl(music);
				music = nullptr;
		}
		break;
	case 6:
		if (popUpOptions != nullptr) {
			app->audio->SetVolume(music->currentValue, true);
		}  
	}
	
	return true;
}

void MainMenu::RenderGUI()
{
	
}
