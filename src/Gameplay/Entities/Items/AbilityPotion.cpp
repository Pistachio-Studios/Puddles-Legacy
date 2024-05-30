#include "Gameplay/Entities/Items/AbilityPotion.h"
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

AbilityPotion::AbilityPotion()
{
	name.Create("AbilityPotion");
}

AbilityPotion::~AbilityPotion() {

}

bool AbilityPotion::Awake() {

	return true;
}

bool AbilityPotion::Start() {
	Potion::Start();

	timer = Timer();

	return true;
}

bool AbilityPotion::Update(float dt)
{

	Player* player;
	player = app->entityManager->GetPlayerEntity();

	if (isCreated && app->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN) {
		usedPotion = true;
		//Funcion ability
		//TODO: Si se puede tener mas de una pocion creada hay que arreglarlo
		isCreated = false;
	}

	Potion::Update(dt);

	return true;
}


bool AbilityPotion::CleanUp() {
	Potion::CleanUp();
	return true;
}

