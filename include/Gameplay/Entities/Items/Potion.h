#ifndef __POTION_H__
#define __POTION_H__

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

class Potion : public Entity
{
public:

	bool startTimer = true;
	Timer timer;

	Potion();
	virtual ~Potion();

	bool Awake() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

	void OnCollision(PhysBody* physA, PhysBody* physB) override;

	void EndCollision(PhysBody* physA, PhysBody* physB);

public:

	SDL_Texture* texture = nullptr;
	const char* texturePath;
	int maxToCreate = 3;
	bool isCreated = false;
	bool usedPotion = false;
	//TODO: Hay que borrar todos los int
	int energyPlantCounter = 4;
	int abilityPlantCounter = 4;
	int healPlantCounter = 4;
	int veloPlantCounter = 4;
private:
	PhysBody* pbody;
};

#endif // __POTION_H__