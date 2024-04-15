#ifndef __SHIELD_H__
#define __SHIELD_H__

#include "Core/Physics.h"
#include "Utils/Timer.h"
#include "Utils/StateMachine.h"

#include <box2d/b2_fixture.h>
#include <SDL.h>
#include <box2d/b2_math.h>

struct SDL_Texture;

class Shield : public Entity
{
public:

	bool startTimer = true;
	Timer timer;

	Shield();

	virtual ~Shield();

	bool Awake() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

	void DrawImGui() override;

	void OnCollision(PhysBody* physA, PhysBody* physB) override;

	void EndCollision(PhysBody* physA, PhysBody* physB) override;

public:
	PhysBody* pbody;
};

#endif // __PLAYER_H__