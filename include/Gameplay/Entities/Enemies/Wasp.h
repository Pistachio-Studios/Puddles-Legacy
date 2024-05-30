#ifndef __WASP_H__
#define __WASP_H__

#include "Core/Physics.h"
#include "Utils/Timer.h"
#include "Utils/StateMachine.h"
#include "Core/Animation.h"
#include "Gameplay/Entities/Entity.h"

#include <box2d/b2_fixture.h>
#include <SDL.h>
#include <box2d/b2_math.h>
#include <Core/Pathfinding.h>

struct SDL_Texture;
class Player;

class Wasp : public Entity
{
public:

	bool startTimer = true;
	Timer timer;

	void Idle(float dt);
	void Move(float dt) override;
	void Attack(float dt) override;

	Wasp();

	virtual ~Wasp();

	bool Awake() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

	void DrawImGui() override;

	void OnCollision(PhysBody* physA, PhysBody* physB) override;

	void EndCollision(PhysBody* physA, PhysBody* physB) override;

	void pathfindingMovement(float dt);

	void OnRaycastHit(b2Fixture* fixture, const b2Vec2& point,
		const b2Vec2& normal, float32 fraction) override;

	bool SaveState(pugi::xml_node& node) override;
	bool LoadState(pugi::xml_node& node) override;

	void moveToSpawnPoint();
	void StopMoving();

public:

	Animation waspIdle, waspAttack, waspMove, waspDamage,waspDeath;

	Timer movementDelay;

	const char* texturePath;
	SDL_Texture* texture = NULL;

	//Movement
	int moveSpeed = 3;
	int noSpeed = 0;
	int updateSpeed;
	float moveForce = 1.0f;
	float maxSpeed = 3.0f;

	int angle;
	b2Vec2 movementDirection;

	//State Machines
	StateMachine<Wasp>* movementFSM = nullptr;

	PhysBody* pbody;

	iPoint spawnPosition;

	float vida=15.0f;
	float dano = 3.0f;
	bool invencible = false;

	Timer reviveTimer;
	Timer attackTimer;
	Player* player;

	int currentPathPos;

	PathFinding* pathfinding;

};

#endif // __WASP_H__