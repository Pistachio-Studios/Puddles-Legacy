#ifndef __BULLET_H__
#define __BULLET_H__

#include "Core/Physics.h"

#include <box2d/b2_fixture.h>
#include <SDL.h>
#include <box2d/b2_math.h>

struct SDL_Texture;

class Timer;

class Bullet {
public:
	PhysBody* pbody = nullptr;
	SDL_Texture* texture = nullptr;
	Timer* timer = nullptr;
	bool active = false;
	iPoint position;

	Bullet();

	void Shoot(b2Vec2 force, SDL_RendererFlip flip);
	void Draw(float angleToPlayer);
	void Update(float angleToPlayer);
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void CleanUp();
};

#endif // __BULLET_H__