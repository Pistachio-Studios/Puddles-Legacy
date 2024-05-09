#include "Gameplay/Entities/Items/AbilityPotion.h"
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
	Item::Start();

	timer = Timer();

	return true;
}

bool AbilityPotion::Update(float dt)
{
	Item::Update(dt);

	//TODO: 
	//if (isPicked) {

	//}

	return true;
}


bool AbilityPotion::CleanUp() {
	Item::CleanUp();
	return true;
}

