#include "Gameplay/Entities/Items/EnergyPotion.h"
#include "Gameplay/Entities/Items/Plant.h"
#include "Gameplay/Entities/Items/Potion.h"
#include "Core/App.h"
#include "Gameplay/Entities/Entity.h"
#include "Gameplay/Entities/Player.h"
#include "Core/Textures.h"
#include "Core/Input.h"
#include "Core/AnimationManager.h"
#include "Core/Render.h"
#include "Gameplay/Scene.h"
#include "Utils/Point.h"
#include "Core/Physics.h"

#include <cmath>
#include <iostream>

#include <box2d/b2_math.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>

#include <imgui.h>

EnergyPotion::EnergyPotion()
{
	name.Create("EnergyPotion");
}

EnergyPotion::~EnergyPotion() {

}

bool EnergyPotion::Awake() {

	return true;
}

bool EnergyPotion::Start() {
	Potion::Start();

	timer = Timer();

	return true;
}

bool EnergyPotion::Update(float dt)
{

	Player* player;
	player = app->entityManager->GetPlayerEntity();

	Potion::Update(dt);

	return true;
}


bool EnergyPotion::CleanUp() {
	Potion::CleanUp();
	return true;
}
