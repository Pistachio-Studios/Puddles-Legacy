#ifndef __SWORD_H__
#define __SWORD_H__

#include "Core/Physics.h"
#include "Utils/Timer.h"

#include <box2d/b2_fixture.h>
#include <SDL.h>
#include <box2d/b2_math.h>

struct SDL_Texture;

class Sword : public Entity
{
public:

	bool startTimer = true;
	Timer timer;

	Sword();

	virtual ~Sword();

	bool Update(float dt) override;

	bool CleanUp() override;

	void DrawImGui() override;

	void OnCollision(PhysBody* physA, PhysBody* physB) override;

	void EndCollision(PhysBody* physA, PhysBody* physB) override;

	void Equip();

	void Store();

public:
	PhysBody* pbody;

	SDL_Texture* texture = nullptr;

	bool active = false;
};

#endif // __PLAYER_H__