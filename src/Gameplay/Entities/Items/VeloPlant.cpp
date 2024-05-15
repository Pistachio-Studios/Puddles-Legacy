#include "Gameplay/Entities/Items/VeloPlant.h"
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

VeloPlant::VeloPlant()
{
	name.Create("VeloPlant");
}

VeloPlant::~VeloPlant() {

}

bool VeloPlant::Awake() {

	return true;
}

bool VeloPlant::Start() {
	Plant::Start();

	timer = Timer();

	return true;
}

bool VeloPlant::Update(float dt)
{
	Plant::Update(dt);

	if (isPicked) {
		energyPlantCounter++;
		isPicked = false;
	}
	LOG("veloPlant: %d", &energyPlantCounter);
	return true;
}

bool VeloPlant::CleanUp() {
	Plant::CleanUp();
	return true;
}