#ifndef __INTRO_H__
#define __INTRO_H__

#include "Gameplay/Scene.h"

struct SDL_Texture;

class Intro : public Scene
{
public:

	// Constructor
	Intro(const SString& name) : Scene(name) {}

	// Destructor
	virtual ~Intro();

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
	SDL_Texture* img;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;

	int music; 
	
	bool exitPressed = false;
};

#endif // __INTRO_H__