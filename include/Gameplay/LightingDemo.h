#ifndef __LIGHTINGDEMO_H__
#define __LIGHTINGDEMO_H__

#include "Gameplay/Scene.h"

class ParticleGenerator;

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
	ParticleGenerator* particleSource1;
	ParticleGenerator* particleSource2;
	ParticleGenerator* particleSource3;
	ParticleGenerator* particleSource4;
};

#endif // __LIGHTINGDEMO_H__