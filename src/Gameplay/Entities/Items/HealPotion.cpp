#include "Gameplay/Entities/Items/HealPotion.h"
#include "Gameplay/Entities/Items/Potion.h"
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

HealPotion::HealPotion()
{
	name.Create("HealPotion");
}

HealPotion::~HealPotion() {

}

bool HealPotion::Awake() {

	return true;
}

bool HealPotion::Start() {
	Potion::Start();

	timer = Timer();

	healPlant = new HealPlant; 

	return true;
}

bool HealPotion::Update(float dt)
{

	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_REPEAT && healPlant->healPlantCounter >= maxToCreate) {
		isCreated = true;
		healPlant->healPlantCounter -= maxToCreate;
		//se guarda en inventario
	}
	if (isCreated && app->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN) {
		usedPotion = true;
		//TODO: funcion heal player
		//TODO: Si se puede tener mas de una pocion creada hay que arreglarlo
		isCreated = false;
	}

	Potion::Update(dt);

	return true;
}


bool HealPotion::CleanUp() {
	Potion::CleanUp();
	return true;
}
