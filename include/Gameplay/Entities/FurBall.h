#ifndef __FURBALL_H__
#define __FURBALL_H__

#include "Gameplay/Entities/Entity.h"
#include "Utils/Point.h"

#include <SDL.h>

struct SDL_Texture;

class FurBall : public Entity
{
public:

	FurBall();
	virtual ~FurBall();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	bool isPicked = false;
	PhysBody* pbody;

private:

	SDL_Texture* texture;
	const char* texturePath;
	
};

#endif // __FURBALL_H__