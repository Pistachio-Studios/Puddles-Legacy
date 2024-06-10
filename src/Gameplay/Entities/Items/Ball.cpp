#include "Gameplay/Entities/Items/Ball.h"
#include "Core/App.h"
#include "Gameplay/Entities/Entity.h"
#include "Gameplay/Entities/Player.h"
#include "Core/Textures.h"
#include "Core/Input.h"
#include "Core/Render.h"
#include "Gameplay/Scene.h"
#include "Utils/Point.h"
#include "Core/Physics.h"
#include "Core/Audio.h"
#include "Utils/StateMachine.h"
#include "Core/SceneManager.h"
#include "Core/Map.h"
#include "Core/AnimationManager.h"

#include <cmath>
#include <iostream>

#include <box2d/b2_math.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>

#include <imgui.h>

Ball::Ball() : Entity(EntityType::BALL)
{
	name.Create("ball");
}

Ball::~Ball() {

}

bool Ball::Awake() {
	return true;
}

bool Ball::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	colour = parameters.attribute("colour").as_int();
	puzzle3Tex = app->tex->Load(parameters.attribute("puzzleTexPath").as_string());

	newPosition = spawnPosition = position;

	timer = Timer();

	pbody = app->physics->CreateCircle(position.x, position.y, 32, bodyType::DYNAMIC);
	pbody->listener = this;
	if (colour == 1) pbody->ctype = ColliderType::BALLV;
	else if (colour == 2) pbody->ctype = ColliderType::BALLL;

	//si quieres dar vueltos como la helice de un helicoptero Boeing AH-64 Apache pon en false la siguiente funcion
	pbody->body->SetFixedRotation(true);
	//pbody->body->GetFixtureList()->SetFriction(25.0f);
	pbody->body->SetLinearDamping(10.0f);

	player = app->entityManager->GetPlayerEntity();


	return true;
}

bool Ball::Update(float dt)
{
	pbody->body->SetTransform(pbody->body->GetPosition(), 0);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	//app->render->DrawRectangle({ position.x - 1, position.y + 8, 36, 18 }, 255, 0, 0);

	SDL_Texture* texturaActiva= puzzle3Tex;
	
	if (colour == 1) {
		rect = { 128, 0, 128, 128 };
	}
	else if (colour == 2) {
		rect = { 0, 0, 128, 128 };
	}

	app->render->DrawTexture(texturaActiva, position.x + 10, position.y + 10, &rect, 1.0f, 0.0f, 0.5f, 2U);

	if (!placed) {
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			if (pickUp) {
				pickUp = false;
				//pbody->body->GetFixtureList()->SetSensor(false);
			}
			else if (isColliding && !pickUp) {
				pickUp = true;
				//pbody->body->GetFixtureList()->SetSensor(true);
			}
		}
	}

	if (pickUp && !placed) {
		pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(player->position.x), PIXEL_TO_METERS(player->position.y-1)), 0);
	}

	return true;
}

void Ball::DrawImGui()
{
	ImGui::Begin("Ball");
	ImGui::Text("Ball Position: %d, %d", position.x, position.y);
	ImGui::Text("Ball Speed: %f", pbody->body->GetLinearVelocity().Length());
	ImGui::End();
}

bool Ball::SaveState(pugi::xml_node& node) {

	pugi::xml_node BallAttributes = node.append_child("Ball");
	BallAttributes.append_attribute("x").set_value(this->position.x);
	BallAttributes.append_attribute("y").set_value(this->position.y);

	return true;
}

bool Ball::LoadState(pugi::xml_node& node)
{
	pbody->body->SetTransform({ PIXEL_TO_METERS(node.child("Ball").attribute("x").as_int()), PIXEL_TO_METERS(node.child("Ball").attribute("y").as_int()) }, node.child("Ball").attribute("angle").as_int());
	// reset player physics
	pbody->body->SetAwake(false);
	pbody->body->SetAwake(true);

	return true;
}

bool Ball::CleanUp() {

	app->tex->UnLoad(puzzle3Tex);
	app->tex->UnLoad(puzzle3Tex);
	app->physics->DestroyBody(pbody);

	return true;
}

void Ball::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype) {

	case ColliderType::PLAYER:
		isColliding = true;
		break;
	case ColliderType::UNKNOWN:
		LOG("Colision UNKNOWN");
		break;
	}
}

void Ball::EndCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype) {

	case ColliderType::PLAYER:
		isColliding = false;
		break;
	case ColliderType::UNKNOWN:
		LOG("Colision UNKNOWN");
		break;
	}
}

void Ball::OnRaycastHit(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {

}
