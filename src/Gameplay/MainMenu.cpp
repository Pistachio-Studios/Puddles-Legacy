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
#include "Core/GuiControlCheckbox.h"
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

	SDL_Rect loadPos = { static_cast<int>(windowW / 2 - 170), static_cast<int>(windowH / 2 + 150), 340,75};
	loadButton = (GuiControlButton*) app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 23, "  Load  ", loadPos, this);

	SDL_Rect optionsPos = { static_cast<int>(windowW / 2 - 170), static_cast<int>(windowH / 2 + 250), 340,75};
	optionsButton = (GuiControlButton*) app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, " Options ", optionsPos, this);

	SDL_Rect creditsPos = { static_cast<int>(windowW / 2 - 170), static_cast<int>(windowH / 2 + 350), 340,75};
	creditsButton = (GuiControlButton*) app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 24, " Credits ", creditsPos, this);

	SDL_Rect exitPos = { static_cast<int>(windowW / 2 - 170), static_cast<int>(windowH / 2 + 450), 340,75};
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
	app->guiManager->RemoveGuiControl(creditsButton);
	app->guiManager->RemoveGuiControl(loadButton);
	app->guiManager->RemoveGuiControl(fx);
	app->guiManager->RemoveGuiControl(vsync);
	app->guiManager->RemoveGuiControl(screenMode);
	app->guiManager->RemoveGuiControl(fullScreen);
	app->guiManager->RemoveGuiControl(borderless);
	app->guiManager->RemoveGuiControl(fullScreenWindow);
	app->guiManager->RemoveGuiControl(resolution);
	app->guiManager->RemoveGuiControl(res7680x4320);
	app->guiManager->RemoveGuiControl(res3840x2160);
	app->guiManager->RemoveGuiControl(res2560x1440);
	app->guiManager->RemoveGuiControl(res1920x1200);
	app->guiManager->RemoveGuiControl(res1920x1080);
	app->guiManager->RemoveGuiControl(res1600x900);
	app->guiManager->RemoveGuiControl(res1280x720);
	app->guiManager->RemoveGuiControl(res1024x768);
	app->guiManager->RemoveGuiControl(res800x600);
	app->guiManager->RemoveGuiControl(res640x480);
	app->guiManager->RemoveGuiControl(saveSlot1);
	app->guiManager->RemoveGuiControl(saveSlot2);
	app->guiManager->RemoveGuiControl(saveSlot3);
	app->guiManager->RemoveGuiControl(saveSlot4);
	app->guiManager->RemoveGuiControl(saveSlot5);
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
	app->guiManager->RemoveGuiControl(creditsButton);
	app->guiManager->RemoveGuiControl(loadButton);
	app->guiManager->RemoveGuiControl(fx);
	app->guiManager->RemoveGuiControl(vsync);
	app->guiManager->RemoveGuiControl(screenMode);
	app->guiManager->RemoveGuiControl(fullScreen);
	app->guiManager->RemoveGuiControl(borderless);
	app->guiManager->RemoveGuiControl(fullScreenWindow);
	app->guiManager->RemoveGuiControl(resolution);
	app->guiManager->RemoveGuiControl(res7680x4320);
	app->guiManager->RemoveGuiControl(res3840x2160);
	app->guiManager->RemoveGuiControl(res2560x1440);
	app->guiManager->RemoveGuiControl(res1920x1200);
	app->guiManager->RemoveGuiControl(res1920x1080);
	app->guiManager->RemoveGuiControl(res1600x900);
	app->guiManager->RemoveGuiControl(res1280x720);
	app->guiManager->RemoveGuiControl(res1024x768);
	app->guiManager->RemoveGuiControl(res800x600);
	app->guiManager->RemoveGuiControl(res640x480);
	app->guiManager->RemoveGuiControl(saveSlot1);
	app->guiManager->RemoveGuiControl(saveSlot2);
	app->guiManager->RemoveGuiControl(saveSlot3);
	app->guiManager->RemoveGuiControl(saveSlot4);
	app->guiManager->RemoveGuiControl(saveSlot5);
	return true;
}

bool MainMenu::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Press Gui Control: %d", control->id);

	switch (control->id) {
	case 1:
		app->sceneManager->ChangeScene("tutorialscene");
        app->guiManager->RemoveGuiControl(crossOButton);
        crossOButton = nullptr;
        app->guiManager->RemoveGuiControl(music);
        music = nullptr;
        break;
	case 2:
		if (popUpOptions == nullptr) {
			// Disable the buttons
			playButton->state = GuiControlState::DISABLED;
			optionsButton->state = GuiControlState::DISABLED;
			exitButton->state = GuiControlState::DISABLED;
			creditsButton->state = GuiControlState::DISABLED;
			loadButton->state = GuiControlState::DISABLED;

			// Create the popUp
			popUpOptions = (GuiControlPopUp*)app->guiManager->CreateGuiControl(GuiControlType::POPUP, 4, "", { 0,0,0,0 }, this);

			SDL_Rect crossOButtonPos = { static_cast<int>(windowW / 2 + 100), static_cast<int>(windowH / 2 - 25), 30, 30 };
			crossOButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "X", crossOButtonPos, this);

			SDL_Rect musicPos = { static_cast<int>(windowW / 2 - 90), static_cast<int>(windowH / 2 + 30), 150, 20 };
			music = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 6, "Music ", musicPos, this, 0, 128);

			SDL_Rect fxPos = { static_cast<int>(windowW / 2 - 90), static_cast<int>(windowH / 2 + 60), 150, 20 };
			fx = (GuiControlSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 7, "FX ", fxPos, this, 0, 128);

			SDL_Rect vsyncPos = { static_cast<int>(windowW / 2 - 90), static_cast<int>(windowH / 2 + 90), 150, 20 };
			vsync = (GuiControlCheckbox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 8, "Vsync ", vsyncPos, this, 0, 1);

			SDL_Rect screenModePos = { static_cast<int>(windowW / 2 - 90), static_cast<int>(windowH / 2 + 120), 150, 20 };
			// TODO make dropdownbox text change depending on the screen mode selected by config.xml
			screenMode = (GuiControlDropDownBox*)app->guiManager->CreateGuiControl(GuiControlType::DROPDOWNBOX, 9, "Screen Mode: ", screenModePos, this, 0, 1);
			
			fullScreen = screenMode->AddOption("Fullscreen", this);
			borderless = screenMode->AddOption("Borderless", this);
			fullScreenWindow = screenMode->AddOption("Fullscreen Window", this);

			SDL_Rect resolutionPos = { static_cast<int>(windowW / 2 - 90), static_cast<int>(windowH / 2 + 150), 150, 20 };
			// TODO make dropdownbox text change depending on the resolution selected by config.xml
			resolution = (GuiControlDropDownBox*)app->guiManager->CreateGuiControl(GuiControlType::DROPDOWNBOX, 10, "Resolution: ", resolutionPos, this, 0, 1);

			res7680x4320	= resolution->AddOption("7680x4320", this);
			res3840x2160	= resolution->AddOption("3840x2160", this);
			res2560x1440	= resolution->AddOption("2560x1440", this);
			res1920x1200	= resolution->AddOption("1920x1200", this);
			res1920x1080	= resolution->AddOption("1920x1080", this);
			res1600x900		= resolution->AddOption("1600x900", this);
			res1280x720		= resolution->AddOption("1280x720", this);
			res1024x768		= resolution->AddOption("1024x768", this);
			res800x600		= resolution->AddOption("800x600", this);
			res640x480		= resolution->AddOption("640x480", this);

		}
		break;
	case 3:
		exitPressed = true;
		break;
	case 5:
		if (crossOButton != nullptr) {
			// Enable the buttons
			playButton->state = GuiControlState::NORMAL;
			optionsButton->state = GuiControlState::NORMAL;
			exitButton->state = GuiControlState::NORMAL;
			creditsButton->state = GuiControlState::NORMAL;
			loadButton->state = GuiControlState::NORMAL;

			// Remove the popUp
			app->guiManager->RemoveGuiControl(popUpOptions);
			popUpOptions = nullptr;
			app->guiManager->RemoveGuiControl(crossOButton);
			crossOButton = nullptr;
			app->guiManager->RemoveGuiControl(music);
			music = nullptr;
			app->guiManager->RemoveGuiControl(fx);
			fx = nullptr;
			app->guiManager->RemoveGuiControl(vsync);
			vsync = nullptr;
			app->guiManager->RemoveGuiControl(screenMode);
			screenMode = nullptr;
			app->guiManager->RemoveGuiControl(fullScreen);
			fullScreen = nullptr;
			app->guiManager->RemoveGuiControl(borderless);
			borderless = nullptr;
			app->guiManager->RemoveGuiControl(fullScreenWindow);
			fullScreenWindow = nullptr;
			app->guiManager->RemoveGuiControl(resolution);
			resolution = nullptr;
			app->guiManager->RemoveGuiControl(res7680x4320);
			res7680x4320 = nullptr;
			app->guiManager->RemoveGuiControl(res3840x2160);
			res3840x2160 = nullptr;
			app->guiManager->RemoveGuiControl(res2560x1440);
			res2560x1440 = nullptr;
			app->guiManager->RemoveGuiControl(res1920x1200);
			res1920x1200 = nullptr;
			app->guiManager->RemoveGuiControl(res1920x1080);
			res1920x1080 = nullptr;
			app->guiManager->RemoveGuiControl(res1600x900);
			res1600x900 = nullptr;
			app->guiManager->RemoveGuiControl(res1280x720);
			res1280x720 = nullptr;
			app->guiManager->RemoveGuiControl(res1024x768);
			res1024x768 = nullptr;
			app->guiManager->RemoveGuiControl(res800x600);
			res800x600 = nullptr;
			app->guiManager->RemoveGuiControl(res640x480);
			res640x480 = nullptr;

			app->guiManager->RemoveGuiControl(popUpLoad);
			popUpLoad = nullptr;
			app->guiManager->RemoveGuiControl(saveSlot1);
			saveSlot1 = nullptr;
			app->guiManager->RemoveGuiControl(saveSlot2);
			saveSlot2 = nullptr;
			app->guiManager->RemoveGuiControl(saveSlot3);
			saveSlot3 = nullptr;
			app->guiManager->RemoveGuiControl(saveSlot4);
			saveSlot4 = nullptr;
			app->guiManager->RemoveGuiControl(saveSlot5);
			saveSlot5 = nullptr;
		}
		break;
	case 6:
		if (popUpOptions != nullptr) {
			app->audio->SetVolume(music->currentValue, true);
		} 
		break;
	case 7:
		if (popUpOptions != nullptr) {
			app->audio->SetVolume(fx->currentValue, false);
		}
		break;
	case 8:
		if (popUpOptions != nullptr) {
			if (vsync->checked) {
				app->render->SetVsync(true);
			}
			else {
				app->render->SetVsync(false);
			}
		}
		break;
	case 9:
		// Dropdown box
		break;
	case 10:
		if (popUpOptions != nullptr) {
			// Set the screen mode to fullscreen
			app->win->SetFullscreen(true);
			screenMode->text = "Screen Mode: Fullscreen";
		}
		break;
	case 11:
		if (popUpOptions != nullptr) {
			// Set the screen mode to borderless
			app->win->SetBorderless(true);
			screenMode->text = "Screen Mode: Borderless";
		}
		break;
	case 12:
		if (popUpOptions != nullptr) {
			// Set the screen mode to fullscreen window
			app->win->SetFullscreenWindow(true);
			screenMode->text = "Screen Mode: Fullscreen Window";
		}
		break;
	case 13:
		if (popUpOptions != nullptr) {
			// Set the resolution to 7680x4320
			app->win->SetResolution(7680, 4320);
			resolution->text = "Resolution: 7680x4320";
		}
		break;
	case 14:
		if (popUpOptions != nullptr) {
			// Set the resolution to 3840x2160
			app->win->SetResolution(3840, 2160);
			resolution->text = "Resolution: 3840x2160";
		}
		break;
	case 15:
		if (popUpOptions != nullptr) {
			// Set the resolution to 2560x1440
			app->win->SetResolution(2560, 1440);
			resolution->text = "Resolution: 2560x1440";
		}
		break;
	case 16:
		if (popUpOptions != nullptr) {
			// Set the resolution to 1920x1200
			app->win->SetResolution(1920, 1200);
			resolution->text = "Resolution: 1920x1200";
		}
		break;
	case 17:
		if (popUpOptions != nullptr) {
			// Set the resolution to 1920x1080
			app->win->SetResolution(1920, 1080);
			resolution->text = "Resolution: 1920x1080";
		}
		break;
	case 18:
		if (popUpOptions != nullptr) {
			// Set the resolution to 1600x900
			app->win->SetResolution(1600, 900);
			resolution->text = "Resolution: 1600x900";
		}
		break;
	case 19:
		if (popUpOptions != nullptr) {
			// Set the resolution to 1280x720
			app->win->SetResolution(1280, 720);
			resolution->text = "Resolution: 1280x720";
		}
		break;
	case 20:
		if (popUpOptions != nullptr) {
			// Set the resolution to 1024x768
			app->win->SetResolution(1024, 768);
			resolution->text = "Resolution: 1024x768";
		}
		break;
	case 21:
		if (popUpOptions != nullptr) {
			// Set the resolution to 800x600
			app->win->SetResolution(800, 600);
			resolution->text = "Resolution: 800x600";
		}
		break;
	case 22:
		if (popUpOptions != nullptr) {
			// Set the resolution to 640x480
			app->win->SetResolution(640, 480);
			resolution->text = "Resolution: 640x480";
		}
		break;
	case 23:
		// Saves popUp
		if (popUpLoad == nullptr) {
			// Disable the buttons
			playButton->state = GuiControlState::DISABLED;
			optionsButton->state = GuiControlState::DISABLED;
			exitButton->state = GuiControlState::DISABLED;
			creditsButton->state = GuiControlState::DISABLED;
			loadButton->state = GuiControlState::DISABLED;

			// Create the popUp
			popUpLoad = (GuiControlPopUp*)app->guiManager->CreateGuiControl(GuiControlType::POPUP, 4, "", { 0,0,0,0 }, this);

			// Create the cross button
			SDL_Rect crossOButtonPos = { static_cast<int>(windowW / 2 + 100), static_cast<int>(windowH / 2 - 25), 30, 30 };
			crossOButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "X", crossOButtonPos, this);

			// Create the saves slots
			saveSlot1 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 25, "Save Slot 1", { static_cast<int>(windowW / 2 - 170), static_cast<int>(windowH / 2 + 50), 150, 20 }, this);
			saveSlot2 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 26, "Save Slot 2", { static_cast<int>(windowW / 2 - 170), static_cast<int>(windowH / 2 + 150), 150, 20 }, this);
			saveSlot3 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 27, "Save Slot 3", { static_cast<int>(windowW / 2 - 170), static_cast<int>(windowH / 2 + 250), 150, 20 }, this);
			saveSlot4 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 28, "Save Slot 4", { static_cast<int>(windowW / 2 - 170), static_cast<int>(windowH / 2 + 350), 150, 20 }, this);
			saveSlot5 = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 29, "Save Slot 5", { static_cast<int>(windowW / 2 - 170), static_cast<int>(windowH / 2 + 450), 150, 20 }, this);
		}
		break;
	case 24:
		// Change to credits scene
		app->sceneManager->ChangeScene("creditsScene");
		break;
	case 25:
		if (popUpLoad != nullptr) {
			// Save Slot 1
			// Go to the save scene
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file("save_game_1.xml");
			if (result) {
				pugi::xml_node sceneManagerNode = doc.child("game_state").child("scene_manager");
				pugi::xml_node currentSceneNode = sceneManagerNode.child("currentScene");

				if (currentSceneNode) {
					const char* sceneName = currentSceneNode.attribute("name").value();
					app->sceneManager->ChangeScene(sceneName);
					currentSlot = 1;
					app->LoadRequest();
				} else {
					// Handle error: currentScene node not found
					LOG("Error: currentScene node not found");
				}
			} else {
				// XML file does not exist, create it
				currentSlot = 1;
				LOG("Creating save_game_1.xml file");
				app->SaveRequest();
			}
		}
		break;
	case 26:
		if (popUpLoad != nullptr) {
			// Save Slot 2
			// Go to the save scene
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file("save_game_2.xml");
			if (result) {
				pugi::xml_node sceneManagerNode = doc.child("game_state").child("scene_manager");
				pugi::xml_node currentSceneNode = sceneManagerNode.child("currentScene");

				if (currentSceneNode) {
					const char* sceneName = currentSceneNode.attribute("name").value();
					app->sceneManager->ChangeScene(sceneName);
					currentSlot = 2;
					app->LoadRequest();
				} else {
					// Handle error: currentScene node not found
					LOG("Error: currentScene node not found");
				}
			} else {
			// XML file does not exist, create it
				currentSlot = 2;
				LOG("Creating save_game_2.xml file");
				app->SaveRequest();
			}
		}
		break;
	case 27:
		if (popUpLoad != nullptr) {
			// Save Slot 3
			// Go to the save scene
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file("save_game_3.xml");
			if (result) {
				pugi::xml_node sceneManagerNode = doc.child("game_state").child("scene_manager");
				pugi::xml_node currentSceneNode = sceneManagerNode.child("currentScene");

				if (currentSceneNode) {
					const char* sceneName = currentSceneNode.attribute("name").value();
					app->sceneManager->ChangeScene(sceneName);
					currentSlot = 3;
					app->LoadRequest();
				} else {
					// Handle error: currentScene node not found
					LOG("Error: currentScene node not found");
				}
			} else {
				// XML file does not exist, create it
				currentSlot = 3;
				LOG("Creating save_game_3.xml file");
				app->SaveRequest();
			}
		}
		break;
	case 28:
		if (popUpLoad != nullptr) {
			// Save Slot 4
			// Go to the save scene
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file("save_game_4.xml");
			if (result) {
				pugi::xml_node sceneManagerNode = doc.child("game_state").child("scene_manager");
				pugi::xml_node currentSceneNode = sceneManagerNode.child("currentScene");

				if (currentSceneNode) {
					const char* sceneName = currentSceneNode.attribute("name").value();
					app->sceneManager->ChangeScene(sceneName);
					currentSlot = 4;
					app->LoadRequest();
				} else {
					// Handle error: currentScene node not found
					LOG("Error: currentScene node not found");
				}
			} else {
				// XML file does not exist, create it
				currentSlot = 4;
				LOG("Creating save_game_4.xml file");
				app->SaveRequest();
			}
		}
		break;
	case 29:
		if (popUpLoad != nullptr) {
			// Save Slot 5
			// Go to the save scene
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file("save_game_5.xml");
			if (result) {
				pugi::xml_node sceneManagerNode = doc.child("game_state").child("scene_manager");
				pugi::xml_node currentSceneNode = sceneManagerNode.child("currentScene");

				if (currentSceneNode) {
					const char* sceneName = currentSceneNode.attribute("name").value();
					app->sceneManager->ChangeScene(sceneName);
					currentSlot = 5;
					app->LoadRequest(); 
				} else {
					// Handle error: currentScene node not found
					LOG("Error: currentScene node not found");
				}
			} else {
				// XML file does not exist, create it
				currentSlot = 5;
				LOG("Creating save_game_5.xml file");
				app->SaveRequest();
			}
		}
		break;
	}
	return true;
}

void MainMenu::RenderGUI()
{
	
}
