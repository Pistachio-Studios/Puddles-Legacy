#ifndef __LEVEL1SCENE_H__
#define __LEVEL1SCENE_H__

#include "Core/GuiControlLabel.h"
#include "Core/Module.h"
#include "Gameplay/Entities/Player.h"
#include "Core/GuiControl.h"
#include "Core/GuiControlButton.h"
#include "Gameplay/Scene.h"

struct SDL_Texture;

class Level1Scene : public Scene
{
public:

	// Constructor
	Level1Scene(const SString& name) : Scene(name) {}

	// Destructor
	virtual ~Level1Scene();

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
	bool winCondition = false;
	Player* player;

private:
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	GuiControlButton* gcButton;
	GuiControlLabel* gcScore;
	GuiControlLabel* gcLives;
	GuiControlLabel* gcTime;

	bool paused = false;
	bool exitPressed = false;

	Timer* playingTime;
};

#endif // __LEVEL1SCENE_H__