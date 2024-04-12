#ifndef __ENEMYBOSS_H__
#define __ENEMYBOSS_H__

#include "Core/Physics.h"
#include "Utils/Timer.h"
#include "Utils/StateMachine.h"

#include <box2d/b2_fixture.h>
#include <SDL.h>
#include <box2d/b2_math.h>

struct SDL_Texture;

class EnemyBoss : public Entity
{
public:

	bool startTimer = true;
	Timer timer;

	EnemyBoss();

	virtual ~EnemyBoss();

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
	float maxSpeed = 2.0f;

	//State Machines
	StateMachine<EnemyBoss>* movementFSM = nullptr;

	PhysBody* pbody;

	int health;
	float damage = 3;
	bool invencible = false;
	Timer reviveTimer;

};

#endif // __ENEMYBOSS_H__