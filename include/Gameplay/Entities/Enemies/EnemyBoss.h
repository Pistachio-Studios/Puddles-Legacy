#ifndef __ENEMYBOSS_H__
#define __ENEMYBOSS_H__

#include "Core/Physics.h"
#include "Utils/Timer.h"
#include "Utils/StateMachine.h"
#include "Core/Animation.h"

#include <box2d/b2_fixture.h>
#include <SDL.h>
#include <box2d/b2_math.h>

#include "Core/Pathfinding.h"

struct SDL_Texture;
class Player;

class EnemyBoss : public Entity
{
public:

	bool startTimer = true;
	Timer timer;

	void Idle(float dt);
	void Move(float dt) override;
	void Attack(float dt) override;

	EnemyBoss();

	virtual ~EnemyBoss();

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

	Animation bossIdle, bossBodyAttack, bossDistanceAttack, bossMove, bossDamage, bossDeath;

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
	StateMachine<EnemyBoss>* movementFSM = nullptr;

	PhysBody* pbody;

	iPoint spawnPosition;

	float vida = 20.0f;
	float dano = 2.0f;

	bool invencible = false;

	Timer reviveTimer;
	Timer attackTimer;
	Timer hurtTimer;
	Player* player;

	int currentPathPos;

	PathFinding* pathfinding;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

};

#endif // __ENEMYBOSS_H__