#include "Gameplay/Entities/Items/VeloPotion.h"
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
	Potion::Update(dt);

	//TODO: Modify maxSpeed to go faster
	
	if (app->input->GetKey(SDL_SCANCODE_3) == KEY_REPEAT && veloPlantCounter >= maxToCreate) {
		isCreated = true;
		veloPlantCounter -= maxToCreate;
	}
	//TODO: La pocion se crea, cuando se usa, si ya no hay mas pociones, iscreated=false
	if (isCreated && app->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN) {
		usedPotion = true;
		//VelocityBuff();
		//TODO: Si se puede tener mas de una pocion creada hay que arreglarlo
		isCreated = false;
	}

	return true;
}


bool VeloPotion::CleanUp() {
	Potion::CleanUp();
	return true;
}

void VeloPotion::VelocityBuff() {
	player->maxSpeed *= 2.0f;
}