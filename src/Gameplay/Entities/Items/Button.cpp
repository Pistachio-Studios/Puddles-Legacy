#include "Gameplay/Entities/Items/Button.h"
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

Button::Button() : Entity(EntityType::BUTTON)
{
	name.Create("button");
}

Button::~Button() {

}

bool Button::Awake() {
	return true;
}

bool Button::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	buttonType = parameters.attribute("type").as_int();
	if (buttonType == 1) {
		buttonTex = app->tex->Load(parameters.attribute("buttonTexPath").as_string());
		buttonPisadoTex = app->tex->Load(parameters.attribute("buttonPisadoTexPath").as_string());
	}
	else if (buttonType == 2) {
		buttonVerde = app->tex->Load(parameters.attribute("buttonVerdeTexPath").as_string());
		buttonAzul = app->tex->Load(parameters.attribute("buttonAzulTexPath").as_string());
		buttonLila = app->tex->Load(parameters.attribute("buttonLilaTexPath").as_string());
		buttonRojo = app->tex->Load(parameters.attribute("buttonRojoTexPath").as_string());
		buttonNaranja = app->tex->Load(parameters.attribute("buttonNaranjaTexPath").as_string());
		colour = 1 + rand() % 5;
	}

	newPosition = spawnPosition = position;

	timer = Timer();

	pbody = app->physics->CreateRectangle(position.x, position.y, 110, 112, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::BUTTON;

	//si quieres dar vueltos como la helice de un helicoptero Boeing AH-64 Apache pon en false la siguiente funcion
	pbody->body->SetFixedRotation(true);
	//pbody->body->GetFixtureList()->SetFriction(25.0f);
	pbody->body->SetLinearDamping(10.0f);

	pbody->body->GetFixtureList()->SetSensor(true);

	player = app->entityManager->GetPlayerEntity();


	return true;
}

bool Button::Update(float dt)
{
	pbody->body->SetTransform(pbody->body->GetPosition(), 0);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	//app->render->DrawRectangle({ position.x - 1, position.y + 8, 36, 18 }, 255, 0, 0);

	SDL_Texture* texturaActiva=nullptr;
	if (buttonType == 1) {
		if (!pisada) texturaActiva = buttonTex;
		else texturaActiva = buttonPisadoTex;
	}
	else if (buttonType == 2) {
		if (pisada) {
			pisada = false;
			colour = 1 + rand() % 5; // Generate a random number between 1 and 5
		}
		switch (colour)
		{
		case 1:
			texturaActiva = buttonVerde;
			break;
		case 2:
			texturaActiva = buttonAzul;
			break;
		case 3:
			texturaActiva = buttonLila;
			break;
		case 4:
			texturaActiva = buttonRojo;
			break;
		case 5:
			texturaActiva = buttonNaranja;
			break;
		}
	}
	app->render->DrawTexture(texturaActiva, position.x-44, position.y-50, 0, 1.0f, 0.0f, 1.0f, 1);

	return true;
}

void Button::DrawImGui()
{
	ImGui::Begin("Button");
	ImGui::Text("Button Position: %d, %d", position.x, position.y);
	ImGui::Text("Button Speed: %f", pbody->body->GetLinearVelocity().Length());
	ImGui::End();
}

bool Button::SaveState(pugi::xml_node& node) {

	pugi::xml_node buttonAttributes = node.append_child("button");
	buttonAttributes.append_attribute("x").set_value(this->position.x);
	buttonAttributes.append_attribute("y").set_value(this->position.y);

	return true;
}

bool Button::LoadState(pugi::xml_node& node)
{
	pbody->body->SetTransform({ PIXEL_TO_METERS(node.child("button").attribute("x").as_int()), PIXEL_TO_METERS(node.child("button").attribute("y").as_int()) }, node.child("button").attribute("angle").as_int());
	// reset player physics
	pbody->body->SetAwake(false);
	pbody->body->SetAwake(true);

	return true;
}

bool Button::CleanUp() {

	app->tex->UnLoad(buttonTex);
	app->tex->UnLoad(buttonPisadoTex);
	app->physics->DestroyBody(pbody);

	return true;
}

void Button::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype) {

	case ColliderType::PLAYER:
		LOG("Collision PLAYER");
		pisada = true;
		break;
	case ColliderType::UNKNOWN:
		LOG("Colision UNKNOWN");
		break;
	}
}

void Button::EndCollision(PhysBody* physA, PhysBody* physB) {

}

void Button::OnRaycastHit(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {

}
