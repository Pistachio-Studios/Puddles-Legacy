#ifndef __MainMenu_H__
#define __MainMenu_H__

#include "Gameplay/Scene.h"
#include "Core/GuiControl.h"
#include "Core/GuiControlButton.h"
#include "Core/GuiControlPopUp.h"
#include "Core/GuiControlSlider.h"

struct SDL_Texture;

class MainMenu : public Scene
{
public:

	// Constructor
	MainMenu(const SString& name) : Scene(name) {}

	// Destructor
	virtual ~MainMenu();

	// Called before the first frame
	bool Enter() override;

	// Called before all Updates
	bool PreUpdate() override;

	// Called each loop iteration
	bool Update(float dt) override;

	// Called before all Updates
	bool PostUpdate() override;

	bool Exit() override;

	// Called before quitting
	bool CleanUp() override;

	// Handles multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control) override;

	void RenderGUI();

public:

private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	GuiControlButton* playButton;
	GuiControlButton* optionsButton;
	GuiControlButton* exitButton;
	GuiControlPopUp* popUpOptions = nullptr;
	GuiControlButton* crossOButton = nullptr;
	GuiControlSlider* music = nullptr;
	
	bool exitPressed = false;

	const char* texturePath;
	SDL_Texture* gameTitle = NULL;
};

#endif // __MainMenu_H__