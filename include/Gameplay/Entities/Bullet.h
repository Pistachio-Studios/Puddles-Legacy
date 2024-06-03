#ifndef __BULLET_H__
#define __BULLET_H__

#include "Core/Physics.h"

#include <box2d/b2_fixture.h>
#include <SDL.h>
#include <box2d/b2_math.h>

struct SDL_Texture;

class Timer;

class Bullet : public Entity
{
public:

	bool startTimer = true;
	Timer timer;

	Bullet();

	virtual ~Bullet();

	bool Awake() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

	void DrawImGui() override;

	void Shoot();

	void OnCollision(PhysBody* physA, PhysBody* physB) override;

	void EndCollision(PhysBody* physA, PhysBody* physB) override;

public:
	PhysBody* pbody;

	float bulletSpeed = 2.0f;

	SDL_Texture* texture = nullptr;
};

#endif // __BULLET_H__