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

	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_REPEAT && player->energyPlantCounter >= maxToCreate) {
		isCreated = true;
		player->energyPlantCounter -= maxToCreate;
	}

	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT && isCreated) {
		if (mouseX >= posX + 50 && mouseX <= posX + 350 && mouseY >= posY + 50 && mouseY <= posY + 350) {
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
				usedPotion = true;
				player->mana = 100; //TODO: Revisar
				if (player->energyPlantCounter <= maxToCreate) {
					isCreated = false;
				}
				timer.Start();
			}
		}
	}

	Potion::Update(dt);

	return true;
}


bool EnergyPotion::CleanUp() {
	Potion::CleanUp();
	return true;
}
