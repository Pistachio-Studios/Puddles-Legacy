#ifndef __FOOD_H__
#define __FOOD_H__

#include "Core/Animation.h"
#include "Gameplay/Entities/Entity.h"
#include "Utils/Point.h"

#include <SDL.h>

struct SDL_Texture;

class FoodItem : public Entity
{
public:

	FoodItem();
	virtual ~FoodItem();

	bool Awake() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

	void OnCollision(PhysBody* physA, PhysBody* physB) override;

public:

	bool isPicked = false;
	SDL_Texture* texture;
	const char* texturePath;

private:
	PhysBody* pbody;

	Animation foodTextures;
};

#endif // __FOOD_H__