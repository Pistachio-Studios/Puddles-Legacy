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

	//TODO: Ahora la pocion se usa dandole al 5 pero se tiene que usar con el boton y lo mismo con todas las pociones
	
	//if (app->input->GetKey(SDL_SCANCODE_TAB) == KEY_REPEAT) { 
	//	if (isCreated) {
	//		if (mouseX >= position.x && mouseX <= pbody->width && mouseY >= position.y && mouseY <= pbody->height) {
	//			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
	//				usedPotion = true;
	//				player->maxSpeed *= 2;
	//				//TODO: Si se puede tener mas de una pocion creada hay que arreglarlo
	//				if (player->veloPlantCounter <= maxToCreate) {
	//					isCreated = false;
	//				}
	//				timer.Start();
	//			}
	//		}
	//	}
	//}
	
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
