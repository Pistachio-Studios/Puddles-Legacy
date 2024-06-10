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
class Bullet;
class ParticleGenerator;

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

	float CalculateAngleToPlayer();

	void shootBullet();

	b2Vec2 calculateForce();

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
	int moveSpeed = 1;
	int noSpeed = 0;
	int updateSpeed;
	float moveForce = 3.0f;
	float maxSpeed = 3.0f;

	int angle;
	b2Vec2 movementDirection;

	//State Machines
	StateMachine<EnemyBoss>* movementFSM = nullptr;

	PhysBody* pbody;

	iPoint spawnPosition;

	ParticleGenerator* damage = nullptr;
	
	float vida = 75.0f;
	float str = 13.0f;
	float inte = 17.0f;
	float def = 15.0f;

	bool invencible = false;

	Timer reviveTimer;
	Timer attackTimer;
	Timer hurtTimer;
	Player* player;

	int currentPathPos;

	PathFinding* pathfinding;
	SDL_RendererFlip flip = SDL_FLIP_NONE;

	//Disparos
	Bullet* bulletArray[10];

	bool isTouchingPlayer = false;

	float angleDegrees;
	bool justShot = false;
	bool dead = false;

	//soundFX
	int bossDamageFx, bossAttackFx, bossDieFx;

};

#endif // __ENEMYBOSS_H__