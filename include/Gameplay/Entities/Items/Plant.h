#ifndef __PLANT_H__
#define __PLANT_H__

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

class Plant : public Entity
{
public:

	bool startTimer = true;
	Timer timer;

	Plant();
	virtual ~Plant();

	bool Awake() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

	void OnCollision(PhysBody* physA, PhysBody* physB) override;

	void EndCollision(PhysBody* physA, PhysBody* physB);

public:

	bool isPicked = false;
	bool touchingPlant = false;
	SDL_Texture* texture1 = nullptr;
	SDL_Texture* texture = nullptr;
	const char* texturePath;
	int type; 
	bool healPlantPicked = false;
	bool veloPlantPicked = false;
	bool energyPlantPicked = false;

private:
	PhysBody* pbody;
};

#endif // __PLANT_H__