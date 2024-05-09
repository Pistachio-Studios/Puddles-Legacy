#ifndef __ABILITYPOTION_H__
#define __ABILITYPOTION_H__

#include "Core/Physics.h"
#include "Utils/Timer.h"
#include "Utils/StateMachine.h"
#include "Gameplay/Entities/Items/Item.h"
#include "Core/GuiControlButton.h"

#include <box2d/b2_fixture.h>
#include <SDL.h>
#include <box2d/b2_math.h>

struct SDL_Texture;
class Player;

class AbilityPotion : public Item
{
public:

	bool startTimer = true;
	Timer timer;

	AbilityPotion();

	virtual ~AbilityPotion();

	bool Awake() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

public:

	const char* texturePath;
	SDL_Texture* texture = NULL;

	PhysBody* pbody;
	Player* player;
};

#endif // __ABILITYPOTION_H__