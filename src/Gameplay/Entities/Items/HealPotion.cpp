#include "Gameplay/Entities/Items/HealPotion.h"
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

	return true;
}

bool HealPotion::Update(float dt)
{	
	
	Player* player;
	player = app->entityManager->GetPlayerEntity();


	/*if (app->input->GetKey(SDL_SCANCODE_TAB) == KEY_REPEAT) {
		if (isCreated) {
			if (mouseX >= position.x && mouseX <= pbody->width && mouseY >= position.y && mouseY <= pbody->height) {
				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) { 
					usedPotion = true;
				}
			}
		}
	}

	if (usedPotion) {
		isCreated = false;
	}*/

	if (usedPotion) {
		usedPotion = false;
	}

	Potion::Update(dt);

	return true;
}


bool HealPotion::CleanUp() {
	Potion::CleanUp();
	return true;
}
