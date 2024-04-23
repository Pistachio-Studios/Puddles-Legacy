#ifndef __TEAMLOGO_H__
#define __TEAMLOGO_H__

#include "Gameplay/Scene.h"
#include "Core/GuiControl.h"
#include "Utils/Timer.h"

struct SDL_Texture;

class TeamLogo : public Scene
{
public:

	// Constructor
	TeamLogo(const SString& name) : Scene(name) {}

	// Destructor
	virtual ~TeamLogo();

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

public:

private:
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;

	const char* texturePath;
	SDL_Texture* teamLogo = NULL;

	Timer* timer = nullptr;
};

#endif // __TEAMLOGO_H__