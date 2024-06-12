#ifndef __LOCO_H__
#define __LOCO_H__

#include "Core/Physics.h"
#include "Utils/Timer.h"
#include "Utils/StateMachine.h"
#include "Gameplay/Entities/Npcs/Npc.h"
#include "Core/GuiControlButton.h"

#include <box2d/b2_fixture.h>
#include <SDL.h>
#include <box2d/b2_math.h>

struct SDL_Texture;

class Loco : public Npc
{
public:

	bool startTimer = true;
	Timer timer;

	Loco();

	virtual ~Loco();

	bool Awake() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

	void OnCollision(PhysBody* physA, PhysBody* physB) override;

	void EndCollision(PhysBody* physA, PhysBody* physB) override;

public:

	const char* texturePath;
	SDL_Texture* texture = NULL;

	GuiControlButton* pressE = nullptr; 

	PhysBody* pbody;
};

#endif // __LOCO_H__