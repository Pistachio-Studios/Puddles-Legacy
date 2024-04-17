#include "Gameplay/Entities/Npcs/Loco.h"
#include "Core/App.h"
#include "Gameplay/Entities/Entity.h"
#include "Core/Textures.h"
#include "Core/Input.h"
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

Loco::Loco()
{
	name.Create("Loco");
}

Loco::~Loco() {

}

bool Loco::Awake() {

	return true;
}

bool Loco::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	timer = Timer();

	return true;
}

bool Loco::Update(float dt)
{
	Npc::Update(dt);

	app->render->DrawTexture(texture, position.x, position.y);

	b2Vec2 mouseWorldPosition = { PIXEL_TO_METERS(app->input->GetMouseX()) + PIXEL_TO_METERS(-app->render->camera.x), PIXEL_TO_METERS(app->input->GetMouseY()) + PIXEL_TO_METERS(-app->render->camera.y) };

	return true;
}


bool Loco::CleanUp() {

	return true;
}
