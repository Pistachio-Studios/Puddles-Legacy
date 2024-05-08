#ifndef __ITEM_H__
#define __ITEM_H__

#include "Core/Animation.h"
#include "Core/Physics.h"
#include "Utils/Timer.h"
#include "Utils/StateMachine.h"
//#include "Point.h"
#ifdef __linux__
#include <SDL.h>
#elif _MSC_VER
#include "SDL.h"
#endif

struct SDL_Texture;

class Item : public Entity
{
public:

	bool startTimer = true;
	Timer timer;

	Item();
	virtual ~Item();

	bool Awake() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

	void OnCollision(PhysBody* physA, PhysBody* physB) override;

	void EndCollision(PhysBody* physA, PhysBody* physB);

public:

	bool isPicked = false;
	bool touchingItem = false;
	SDL_Texture* texture1 = nullptr;
	SDL_Texture* texture = nullptr;
	const char* texturePath;

private:
	PhysBody* pbody;
};

#endif // __ITEM_H__