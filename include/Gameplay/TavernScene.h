#ifndef __TAVERNSCENE_H__
#define __TAVERNSCENE_H__

#include "Core/GuiControlLabel.h"
#include "Core/Module.h"
#include "Gameplay/Entities/Player.h"
#include "Core/GuiControl.h"
#include "Core/GuiControlButton.h"
#include "Core/GuiControlPopUp.h"
#include "Gameplay/Scene.h"

struct SDL_Texture;

class TavernScene : public Scene
{
public:

	// Constructor
	TavernScene(const SString& name) : Scene(name) {}

	// Destructor
	virtual ~TavernScene();

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
	SDL_Texture* cauldronTex;
	SDL_Texture* cauldronSelectTex;
	SDL_Texture* prueba;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	GuiControlButton* gcButton;
	GuiControlLabel* gcScore;
	GuiControlLabel* gcLives;
	GuiControlLabel* gcTime;

	GuiControlButton* gcResume;
	GuiControlButton* gcSettings;
	GuiControlButton* gcBackToTitle;
	GuiControlButton* gcExit;

	GuiControlButton* cauldronSelectExit;
	GuiControlButton* cauldronExit;
	GuiControlButton* cauldronCreate;
	GuiControlPopUp* cauldron = nullptr;
	GuiControlPopUp* cauldronSelect = nullptr;

	bool paused = false;
	bool exitPressed = false;
	bool selectExitPressed = false;
	bool cauldronExitPressed = false;
	bool cauldronCreatePressed = false;

	Timer* playingTime;
};

#endif // __TAVERNSCENE_H__