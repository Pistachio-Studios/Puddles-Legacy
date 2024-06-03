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
	return true;
}


bool Loco::CleanUp() {
	Npc::CleanUp();
	return true;
}
