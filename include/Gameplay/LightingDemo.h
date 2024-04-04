#ifndef __LIGHTINGDEMO_H__
#define __LIGHTINGDEMO_H__

#include "Core/Lighting.h"
#include "Gameplay/Scene.h"

const int MAX_LIGHTS = 10;

struct SDL_Texture;

class LightingDemo : public Scene
{
public:

	// Constructor
	LightingDemo(const SString& name) : Scene(name) {}

	// Destructor
	virtual ~LightingDemo();

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

private:

	SDL_Texture* background = nullptr;

	Light* lights[MAX_LIGHTS];
};

#endif // __LIGHTINGDEMO_H__