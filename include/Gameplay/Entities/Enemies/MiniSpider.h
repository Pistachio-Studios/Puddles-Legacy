#ifndef __MINISPIDER_H__
#define __MINISPIDER_H__

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

class ParticleGenerator;

class MiniSpider : public Entity
{
public:

	bool startTimer = true;
	Timer timer;

	void Idle(float dt);
	void Move(float dt) override;
	void Attack(float dt) override;

	MiniSpider();

	virtual ~MiniSpider();

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

	Animation spiderIdle, spiderAttack, spiderMove, spiderDamage, spiderDeath;

	Timer movementDelay;

	const char* texturePath;
	SDL_Texture* texture = NULL;

	//Movement
	int moveSpeed = 2;
	int noSpeed = 0;
	int updateSpeed;
	float moveForce = 1.0f;
	float maxSpeed = 2.0f;

	ParticleGenerator* damage = nullptr;

	float vida = 5.0f;
	float dano = 1.0f;

	int angle;
	b2Vec2 movementDirection;

	//State Machines
	StateMachine<MiniSpider>* movementFSM = nullptr;

	PhysBody* pbody;

	iPoint spawnPosition;

	bool invencible = false;

	Timer reviveTimer;
	Timer attackTimer;
	Player* player;

	int currentPathPos;

	//soundFX
	int damageFx,attackFx,dieFx;

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	PathFinding* pathfinding;

	bool isTouchingPlayer = false;

	bool dead = false;

	ParticleGenerator* particleSource1;

};

#endif // __MINISPIDER_H__