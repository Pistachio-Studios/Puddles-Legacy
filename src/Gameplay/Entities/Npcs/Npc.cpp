#include "Gameplay/Entities/Npcs/Npc.h"
#include "Core/App.h"
#include "Core/Window.h"
#include "Gameplay/Entities/Entity.h"
#include "Core/Textures.h"
#include "Core/Input.h"
#include "Core/Animation.h"
#include "Core/Render.h"
#include "Gameplay/Scene.h"
#include "Core/SceneManager.h"
#include "Utils/Point.h"
#include "Utils/Log.h"

#include "Core/DialogManager.h"

#include <box2d/b2_math.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>

#include <imgui.h>

Npc::Npc() : Entity(EntityType::NPC)
{
	name.Create("NPC");
}

Npc::~Npc() {

}

bool Npc::Awake() {

	return true;
}

bool Npc::Start() {

	timer = Timer();

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	type = parameters.attribute("type").as_int();

	if (type == 2) {
		path = parameters.attribute("texturepath").as_string();
		texture = app->tex->Load(path);
	}
 
	texture2 = app->tex->Load("Assets/Textures/PressE.png");
	texture3 = app->tex->Load("Assets/Textures/ClickHere.png");

	pbody = app->physics->CreateRectangle(position.x, position.y, 128, 256, bodyType::STATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::NPC;
	if(mouseMode)pbody->body->GetFixtureList()->SetSensor(true);

	npcSensor = app->physics->CreateRectangleSensor(position.x, position.y, 456, 456, bodyType::STATIC);
	npcSensor->listener = this;
	npcSensor->ctype = ColliderType::NPC;

	return true;
}

bool Npc::Update(float dt)
{
	mouseWorldPosition = { PIXEL_TO_METERS(app->input->GetMouseX()) + PIXEL_TO_METERS(-app->render->camera.x), PIXEL_TO_METERS(app->input->GetMouseY()) + PIXEL_TO_METERS(-app->render->camera.y) };
	pbody->body->SetTransform(pbody->body->GetPosition(), 0);

	int width, height;
	SDL_QueryTexture(anim.texture, NULL, NULL, &width, &height);

	if (type == 1) {
		if (app->sceneManager->GetCurrentScene()->name == "tavernscene") { 
			int mouseX = METERS_TO_PIXELS(mouseWorldPosition.x);
			int mouseY = METERS_TO_PIXELS(mouseWorldPosition.y);
			if (touchingNpc) {
				app->render->DrawTexture(texture3, position.x - 60, position.y - 180);
				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
					//sale dialogo
					app->dialogManager->StartDialog(1);
					app->render->camera.lerpSpeed = 0.0f;
				}
			}
		}
		else {
			if (touchingNpc) {
				app->render->DrawTexture(texture2, position.x - 60, position.y - 180);
				if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
					//sale dialogo
					app->dialogManager->StartDialog(1);
					//if the npc is bounty change his bool talked to true
					if (name == "Loco") {
						bountytalked = true;
					}
					if (name == "Tabernero") {
						klaustalked = true;
					}
				}
			}
		}
		app->render->DrawTexture(anim.texture, position.x - 100, position.y - 200, &anim.GetCurrentFrame());
		anim.Update(dt);
	}
	else {
		app->render->DrawTexture(texture, position.x - 128, position.y - 128);
	}



	return true;
}


bool Npc::SaveState(pugi::xml_node& node) {
	return true;
}

bool Npc::LoadState(pugi::xml_node& node) {
	return true;
}

bool Npc::CleanUp() {

	app->tex->UnLoad(texture2);
	app->tex->UnLoad(texture3);
	app->physics->DestroyBody(pbody);

	return true;
}

void Npc::OnCollision(PhysBody* physA, PhysBody* physB) {
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		touchingNpc = true; 
		break;
	}
}

void Npc::EndCollision(PhysBody* physA, PhysBody* physB)
{
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;
	        
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		touchingNpc = false;
		break;
	}
}