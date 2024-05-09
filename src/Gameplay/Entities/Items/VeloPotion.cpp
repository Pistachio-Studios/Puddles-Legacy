#include "Gameplay/Entities/Items/VeloPotion.h"
#include "Gameplay/Entities/Items/Item.h"
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
	Item::Start();

	timer = Timer();

	return true;
}

bool VeloPotion::Update(float dt)
{
	Item::Update(dt);

	//TODO: Modify maxSpeed to go faster
	//if (isPicked) {
	//	VelocityBuff();
	//}

	return true;
}


bool VeloPotion::CleanUp() {
	Item::CleanUp();
	return true;
}

void VeloPotion::VelocityBuff() {
	player->maxSpeed *= 2.0f;
}