#include "Gameplay/Entities/Npcs/Tabernero.h"
#include "Core/App.h"
#include "Gameplay/Entities/Entity.h"
#include "Core/Textures.h"
#include "Core/Input.h"
#include "Core/AnimationManager.h"
#include "Core/Render.h"
#include "Gameplay/Scene.h"
#include "Utils/Point.h"
#include "Core/Physics.h"
#include "Gameplay/Entities/Player.h"
#include "Core/DialogManager.h"

#include <cmath>
#include <iostream>

#include <box2d/b2_math.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>

#include <imgui.h>

Tabernero::Tabernero()
{
	name.Create("Tabernero");
}

Tabernero::~Tabernero() {

}

bool Tabernero::Awake() {

	return true;
}

bool Tabernero::Start() {
	Npc::Start();

	anim = *app->animationManager->GetAnimByName("klaus_idle_1");
	anim.speed = 1.5f;

	timer = Timer();

	return true;
}

bool Tabernero::Update(float dt)
{
	Npc::Update(dt);

	// TODO add to bestiary when player interacts with tabernero for the first time
	if (klaustalked) {
		app->entityManager->GetPlayerEntity()->bestiary->klausUnlocked = true;
	}
	return true;
}

bool Tabernero::CleanUp() {
	Npc::CleanUp();
	return true; 
}

void Tabernero::OnCollision(PhysBody* physA, PhysBody* physB) {
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		touchingNpc = true; 
		break;
	}
}

void Tabernero::EndCollision(PhysBody* physA, PhysBody* physB)
{
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;
	        
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		touchingNpc = false;
		break;
	}
}