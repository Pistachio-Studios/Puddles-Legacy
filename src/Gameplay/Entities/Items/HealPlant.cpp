#include "Gameplay/Entities/Items/HealPlant.h"
#include "Gameplay/Entities/Items/Plant.h"
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

HealPlant::HealPlant()
{
	name.Create("HealPlant");
}

HealPlant::~HealPlant() {

}

bool HealPlant::Awake() {

	return true;
}

bool HealPlant::Start() {
	Plant::Start();

	timer = Timer();

	return true;
}

bool HealPlant::Update(float dt)
{
	Plant::Update(dt);

	if (healPlantPicked) {
		healPlantCounter++;
		healPlantPicked = false;
	}

	LOG("healPlant: %d", &healPlantCounter);
	return true;
}

bool HealPlant::CleanUp() {
	Plant::CleanUp();
	return true;
}