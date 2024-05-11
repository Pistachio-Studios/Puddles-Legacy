#ifndef __STAFF_H__
#define __STAFF_H__

#include "Core/Physics.h"
#include "Utils/Point.h"
#include "Utils/Timer.h"

#include <box2d/b2_fixture.h>
#include <SDL.h>
#include <box2d/b2_math.h>

struct SDL_Texture;

class Magic {
	public:
	PhysBody* pbody = nullptr;
	SDL_Texture* texture = nullptr;
	Timer* timer = nullptr;
	bool active = false;
	iPoint position;

	Magic();

	void Throw();
	void Draw();
	void Update();
	void OnCollision(PhysBody* physA, PhysBody* physB);
	void CleanUp();
};

class Staff : public Entity
{
public:

	bool startTimer = true;
	Timer timer;

	Staff();

	virtual ~Staff();

	bool Awake() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

	void DrawImGui() override;

	void OnCollision(PhysBody* physA, PhysBody* physB) override;

	void EndCollision(PhysBody* physA, PhysBody* physB) override;

	void ThrowSpell();

public:
	PhysBody* pbody;

	SDL_Texture* texture = nullptr;

	Magic magic[10];

	int magicIndex = 0;
};

#endif // __PLAYER_H__