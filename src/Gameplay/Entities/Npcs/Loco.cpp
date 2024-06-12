#include "Gameplay/Entities/Npcs/Loco.h"
#include "Core/App.h"
#include "Gameplay/Entities/Entity.h"
#include "Core/Textures.h"
#include "Core/Input.h"
#include "Core/AnimationManager.h"
#include "Core/Render.h"
#include "Gameplay/Entities/Npcs/Npc.h"
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

Loco::Loco()
{
	name.Create("Loco");
}

Loco::~Loco() {

}

bool Loco::Awake() {

	return true;
}

bool Loco::Start() {
	Npc::Start();

	anim = *app->animationManager->GetAnimByName("mono_idle_2_izquierda");
	anim.speed = 1.5f;

	timer = Timer();

	return true;
}

bool Loco::Update(float dt)
{
	Npc::Update(dt);

	// TODO add to bestiary when player interacts with tabernero for the first time
	if (touchingNpc) {
		app->render->DrawTexture(texture2, position.x - 60, position.y - 180);
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			//sale dialogo
			bountytalked = true;
			app->dialogManager->StartDialog(32,36);
			app->entityManager->GetPlayerEntity()->bestiary->bountyUnlocked = true;

		}
	}

	return true;
}

void Loco::OnCollision(PhysBody* physA, PhysBody* physB) {
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		touchingNpc = true; 
		break;
	}
}

void Loco::EndCollision(PhysBody* physA, PhysBody* physB)
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

bool Loco::CleanUp() {
	Npc::CleanUp();
	return true;
}
