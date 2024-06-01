#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Core/Physics.h"
#include "Utils/Timer.h"
#include "Utils/StateMachine.h"

#include <box2d/b2_fixture.h>
#include <SDL.h>
#include <box2d/b2_math.h>

struct SDL_Texture;
class Sword;
class Staff;

enum PlayerClass
{
	KNIGHT,
	WIZARD
};

class Player : public Entity
{
public:

	bool startTimer = true;
	Timer timer;

	Player();
	
	virtual ~Player();

	bool Awake() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

	void DrawImGui() override;

	void OnCollision(PhysBody* physA, PhysBody* physB) override;

	void EndCollision(PhysBody* physA, PhysBody* physB) override;

	void OnRaycastHit(b2Fixture* fixture, const b2Vec2& point,
                       const b2Vec2& normal, float32 fraction) override;

	bool SaveState(pugi::xml_node& node) override;
	bool LoadState(pugi::xml_node& node) override;

public:

	const char* texturePath;
	SDL_Texture* texture = NULL;

	//Movement
	float moveForce = 1.0f;
	float maxSpeed = 5.0f;

	//State Machines
	StateMachine<Player>* movementFSM = nullptr;
	StateMachine<Player>* combatFSM = nullptr;

	PhysBody* pbody;

	b2Vec2 lookingDir;
	float lookingAngle = 0;

	PlayerClass currentClass = KNIGHT;

	Sword* swordEntity = nullptr;
    Staff* staffEntity = nullptr;


	uint mana = 100;
	uint livesPlayer = 10;
	int totalLivesPlayer;
	bool deadPlayer = false;

	int healPlantCounter = 0;
	int energyPlantCounter = 0;
	int veloPlantCounter = 0;

	Timer playerHurtCultdown;
	Timer dashTimer;
	float dashCultdown = 5.0f;
};

#endif // __PLAYER_H__