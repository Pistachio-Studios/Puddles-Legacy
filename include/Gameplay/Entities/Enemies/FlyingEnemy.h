#ifndef __FLYINGENEMY_H__
#define __FLYINGENEMY_H__

#include "Core/Physics.h"
#include "Utils/Timer.h"
#include "Utils/StateMachine.h"

#include <box2d/b2_fixture.h>
#include <SDL.h>
#include <box2d/b2_math.h>

struct SDL_Texture;
class Player;

class FlyingEnemy : public Entity
{
public:

	bool startTimer = true;
	Timer timer;

	void Idle(float dt);
	void Move(float dt) override;
	void Attack(float dt) override;

	FlyingEnemy();

	virtual ~FlyingEnemy();

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

	Timer movementDelay;

	const char* texturePath;
	SDL_Texture* texture = NULL;

	//Movement
	int moveSpeed = 2;
	int noSpeed = 0;
	int updateSpeed;
	float moveForce = 1.0f;
	float maxSpeed = 2.0f;

	int angle;
	b2Vec2 movementDirection;

	//State Machines
	StateMachine<FlyingEnemy>* movementFSM = nullptr;

	PhysBody* pbody;

	iPoint spawnPosition;

	int health;
	float damage = 3;
	bool invencible = false;

	Timer reviveTimer;
	Timer attackTimer;
	Player* player;

	int currentPathPos;


};

#endif // __FLYINGENEMY_H__