#include "Gameplay/Entities/Items/VeloPotion.h"
#include "Gameplay/Entities/Items/Potion.h"
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


VeloPotion::VeloPotion()
{
	name.Create("VeloPotion");
}

VeloPotion::~VeloPotion() {

}

bool VeloPotion::Awake() {

	return true;
}

bool VeloPotion::Start() {
	Potion::Start();

	timer = Timer();

	return true;
}

bool VeloPotion::Update(float dt)
{

	//TODO: Modify maxSpeed to go faster
	Player* player;
	player = app->entityManager->GetPlayerEntity();

	if (app->input->GetKey(SDL_SCANCODE_3) == KEY_REPEAT && player->veloPlantCounter >= maxToCreate) {
		bool counter = true;
		isCreated = true;
		player->veloPlantCounter -= maxToCreate;
	}
	
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT && isCreated) {
		if (mouseX >= posX + 50 && mouseX <= posX + 350 && mouseY >= posY + 50 && mouseY <= posY + 350) {
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
				usedPotion = true;
				player->maxSpeed *= 2;
				if (player->veloPlantCounter <= maxToCreate) {
					isCreated = false;
				}
				timer.Start();
			}
		}
	}

	if (usedPotion && timer.ReadSec() >= potionDuration) {
		player->maxSpeed /= 2; // Restore original speed after duration
		usedPotion = false;
	}

	Potion::Update(dt);

	return true;
}


bool VeloPotion::CleanUp() {
	Potion::CleanUp();
	return true;
}
