#include "Gameplay/Entities/EnemyBoss.h"
#include "Core/App.h"
#include "Gameplay/Entities/Entity.h"
#include "Core/Textures.h"
#include "Core/Input.h"
#include "Core/Render.h"
#include "Gameplay/Scene.h"
#include "Utils/Point.h"
#include "Core/Physics.h"
#include "Utils/StateMachine.h"


//#include "Gameplay/States/Enemy/EnemyIdleState.hpp"
//#include "Gameplay/States/Enemy/EnemyMoveState.hpp"

#include <cmath>
#include <iostream>

#include <box2d/b2_math.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>

#include <imgui.h>

EnemyBoss::EnemyBoss() : Entity(EntityType::ENEMYBOSS)
{
	name.Create("EnemyBoss");
}

EnemyBoss::~EnemyBoss() {

}

bool EnemyBoss::Awake() {

	return true;
}

bool EnemyBoss::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	timer = Timer();

	pbody = app->physics->CreateRectangle(position.x, position.y, 20, 10, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	//si quieres dar vueltos como la helice de un helicoptero Boeing AH-64 Apache pon en false la siguiente funcion
	pbody->body->SetFixedRotation(true);
	//pbody->body->GetFixtureList()->SetFriction(25.0f);
	pbody->body->SetLinearDamping(10.0f);


	return true;
}

bool EnemyBoss::Update(float dt)
{
	pbody->body->SetTransform(pbody->body->GetPosition(), 0);

	app->render->DrawLine(METERS_TO_PIXELS(pbody->body->GetPosition().x), METERS_TO_PIXELS(pbody->body->GetPosition().y), METERS_TO_PIXELS(pbody->body->GetPosition().x) + pbody->body->GetLinearVelocity().x * 10, METERS_TO_PIXELS(pbody->body->GetPosition().y) + +pbody->body->GetLinearVelocity().y * 10, 255, 255, 0);


	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	if (debug) {
		if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN) {
			freeCam = !freeCam;
		}
	}

	app->render->DrawRectangle({ position.x,position.y,20,10 }, 255, 255, 255);

	/* 	app->render->DrawTexture(currentAnimation->texture, position.x - 9, position.y - 9, &currentAnimation->GetCurrentFrame(), 1.0f, pbody->body->GetAngle()*RADTODEG, flip);

		currentAnimation->Update(dt); */



	return true;
}

void EnemyBoss::DrawImGui()
{
	ImGui::Begin("Enemy");
	ImGui::Text("Enemy Position: %d, %d", position.x, position.y);
	ImGui::Text("Enemy Speed: %f", pbody->body->GetLinearVelocity().Length());
	ImGui::SliderFloat("max speed", &maxSpeed, 1.0f, 10.0f);
	ImGui::SliderFloat("move force", &moveForce, 1.0f, 10.0f);
	ImGui::End();
}

bool EnemyBoss::SaveState(pugi::xml_node& node) {

	pugi::xml_node playerAttributes = node.append_child("EnemyBoss");
	playerAttributes.append_attribute("x").set_value(this->position.x);
	playerAttributes.append_attribute("y").set_value(this->position.y);

	return true;

}

bool EnemyBoss::LoadState(pugi::xml_node& node)
{
	pbody->body->SetTransform({ PIXEL_TO_METERS(node.child("boss").attribute("x").as_int()), PIXEL_TO_METERS(node.child("boss").attribute("y").as_int()) }, node.child("boss").attribute("angle").as_int());
	// reset player physics
	pbody->body->SetAwake(false);
	pbody->body->SetAwake(true);

	return true;
}

bool EnemyBoss::CleanUp() {

	app->tex->UnLoad(texture);
	app->physics->DestroyBody(pbody);

	return true;
}

void EnemyBoss::OnCollision(PhysBody* physA, PhysBody* physB) {

}

void EnemyBoss::EndCollision(PhysBody* physA, PhysBody* physB) {

}

void EnemyBoss::OnRaycastHit(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {

}
