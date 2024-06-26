#ifndef BUTTON_H
#define BUTTON_H

#include "Core/Animation.h"
#include "Core/Physics.h"
#include "Utils/Timer.h"
#include "Utils/StateMachine.h"

#include <box2d/b2_fixture.h>
#include <SDL.h>
#include <box2d/b2_math.h>
#include <Core/Pathfinding.h>

struct SDL_Texture;
class Player;
class Ball;

class Button : public Entity {

public:

	bool startTimer = true;
	Timer timer;

	Button();

	virtual ~Button();

	bool Awake() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

	void DrawImGui() override;

	void OnCollision(PhysBody * physA, PhysBody * physB) override;

	void EndCollision(PhysBody * physA, PhysBody * physB) override;

	void OnRaycastHit(b2Fixture * fixture, const b2Vec2 & point,
		const b2Vec2 & normal, float32 fraction) override;

	bool SaveState(pugi::xml_node & node) override;

	bool LoadState(pugi::xml_node & node) override;


public:

    SDL_Texture* buttonTex = nullptr;
    SDL_Texture* buttonPisadoTex = nullptr;
	SDL_Texture* buttonVerde = nullptr;
	SDL_Texture* buttonAzul = nullptr;
	SDL_Texture* buttonLila = nullptr;
	SDL_Texture* buttonRojo = nullptr;
	SDL_Texture* buttonNaranja = nullptr;
	SDL_Texture* puzzle3Tex = nullptr;

    const char* texturePath;

    bool isColliding = false;

    PhysBody* pbody;

	iPoint spawnPosition;

	SDL_Rect rect = { 0, 0, 0, 0 };

	Player* player;

	Ball* ball;

	int buttonType;

	//bool changeColor;

	int colour;

	bool pisada = false;
	bool isTouchingBall = false;

};

#endif // BUTTON_H