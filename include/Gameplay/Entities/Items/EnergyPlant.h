#ifndef __ENERGYPLANT_H__
#define __ENERGYPLANT_H__

#include "Core/Physics.h"
#include "Utils/Timer.h"
#include "Utils/StateMachine.h"
#include "Gameplay/Entities/Items/Plant.h"
#include "Core/GuiControlButton.h"

#include <box2d/b2_fixture.h>
#include <SDL.h>
#include <box2d/b2_math.h>

struct SDL_Texture;

class EnergyPlant : public Plant
{
public:

	bool startTimer = true;
	Timer timer;

	EnergyPlant();

	virtual ~EnergyPlant();

	bool Awake() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

public:

	const char* texturePath;
	SDL_Texture* texture = NULL;

	int energyPlantCounter = 0;

	PhysBody* pbody;
};

#endif // __ENERGYPLANT_H__