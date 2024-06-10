#ifndef __FORESTSCENE_H__
#define __FORESTSCENE_H__

#include "Core/GuiControlLabel.h"
#include "Core/Module.h"
#include "Core/GuiControl.h"
#include "Core/GuiControlButton.h"
#include "Gameplay/Entities/Player.h"
#include "Gameplay/Entities/Enemies/EnemyBoss.h"
#include "Gameplay/Entities/Enemies/MiniSpider.h"
#include "Gameplay/Entities/Enemies/Wasp.h"
#include "Gameplay/Scene.h"

struct SDL_Texture;
class Button;
class Ball;

class ForestScene : public Scene
{
public:

	// Constructor
	ForestScene(const SString& name) : Scene(name) {}

	// Destructor
	virtual ~ForestScene();

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
	EnemyBoss* enemyboss;
	MiniSpider* minispider;
	Wasp* wasp;

private:
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	GuiControlButton* gcButton;
	GuiControlLabel* gcScore;
	GuiControlLabel* gcLives;
	GuiControlLabel* gcTime;

	GuiControlButton* gcResume;
	GuiControlButton* gcSave;
	GuiControlButton* gcSettings;
	GuiControlButton* gcBackToTitle;
	GuiControlButton* gcExit;

	bool paused = false;
	bool exitPressed = false;

	Timer* playingTime;

	Button* buttonList[6];

	Button* buttonColourList[5];

	Button* buttonBallList[2];

	Ball* ballList[2];

	int puzzleFx;

	PhysBody* bushPbody;
	SDL_Texture* bush = nullptr;
	bool door1Closed = false;
	bool door3Closed = false;
	bool puzzle1 = false;
	bool puzzle2 = false;
	bool puzzle3 = false;

};

#endif // __FORESTSCENE_H__