#ifndef __HEALPOTION_H__
#define __HEALPOTION_H__

#include "Core/Physics.h"
#include "Utils/Timer.h"
#include "Utils/StateMachine.h"
#include "Gameplay/Entities/Items/Potion.h"
#include "Core/GuiControlButton.h"

#include <box2d/b2_fixture.h>
#include <SDL.h>
#include <box2d/b2_math.h>

struct SDL_Texture;

class HealPotion : public Potion
{
public:

	bool startTimer = true;
	Timer timer;

	HealPotion();

	virtual ~HealPotion();

	bool Awake() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

public:

	const char* texturePath;
	SDL_Texture* texture = NULL;

	PhysBody* pbody;
};

#endif // __HEALPOTION_H__