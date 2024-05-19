#include "Gameplay/Entities/Items/EnergyPlant.h"
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

EnergyPlant::EnergyPlant()
{
	name.Create("EnergyPlant");
}

EnergyPlant::~EnergyPlant() {

}

bool EnergyPlant::Awake() {

	return true;
}

bool EnergyPlant::Start() {
	Plant::Start();

	timer = Timer();

	return true;
}

bool EnergyPlant::Update(float dt)
{
	Plant::Update(dt);

	if (energyPlantPicked) {
		energyPlantCounter++;
		energyPlantPicked = false;
	}
	LOG("energyPlant: %d", &energyPlantCounter);
	return true;
}

bool EnergyPlant::CleanUp() {
	Plant::CleanUp();
	return true;
}