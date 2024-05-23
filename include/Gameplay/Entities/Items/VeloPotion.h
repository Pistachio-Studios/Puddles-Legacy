#ifndef __VELOPOTION_H__
#define __VELOPOTION_H__

#include "Core/Physics.h"
#include "Utils/Timer.h"
#include "Utils/StateMachine.h"
#include "Gameplay/Entities/Items/Potion.h"
#include "Core/GuiControlButton.h"

#include <box2d/b2_fixture.h>
#include <SDL.h>
#include <box2d/b2_math.h>

struct SDL_Texture;
class VeloPlant;

class VeloPotion : public Potion
{
public:

	bool startTimer = true;
	Timer timer;

	VeloPotion();

	virtual ~VeloPotion();

	bool Awake() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

public:

	const char* texturePath;
	SDL_Texture* texture = NULL;

	PhysBody* pbody;
	const float potionDuration = 5.0f; // Duration of the potion effect in seconds
};

#endif // __VELOPOTION_H__