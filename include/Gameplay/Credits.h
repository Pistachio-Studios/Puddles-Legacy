#ifndef __CREDITS_H__
#define __CREDITS_H__

#include "Gameplay/Scene.h"
#include "Core/Render.h"
#include "Core/GuiControl.h"
#include "Core/GuiControlButton.h"
#include "Core/GuiControlPopUp.h"
#include "Utils/Timer.h"

struct SDL_Texture;

class Credits : public Scene
{
public:

	// Constructor
	Credits(const SString& name) : Scene(name) {}

	// Destructor
	virtual ~Credits();

	// Called before the first frame
	bool Enter() override;

	// Called before all Updates
	bool PreUpdate() override;

	// Called each loop iteration
	bool Update(float dt) override;

	// Called before all Updates
	bool PostUpdate() override;

	bool Exit() override;

	// Handles multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control) override;

public:

private:
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;

	const char* texturePath;
	SDL_Texture* credits = NULL;

	Timer* timer = nullptr;

	GuiControlButton* selectBack;
	bool selectBackPressed = false;

	//Audio fx
	int FxId;
};

#endif // __CREDITS_H__