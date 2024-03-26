#ifndef __MAINMENU_H__
#define __MAINMENU_H__

#include "Gameplay/Scene.h"
#include "Core/GuiControl.h"
#include "Core/GuiControlButton.h"

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

	bool exitPressed = false;
};

#endif // __MAINMENU_H__