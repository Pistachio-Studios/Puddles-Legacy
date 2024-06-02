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
 
	texture2 = app->tex->Load("Assets/Textures/pressE.png");
	texture3 = app->tex->Load("Assets/Textures/click.png");

	pbody = app->physics->CreateRectangle(position.x, position.y, 128, 128, bodyType::STATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::NPC;

	npcSensor = app->physics->CreateRectangleSensor(position.x, position.y, 256, 256, bodyType::STATIC);
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
			if (mouseX > position.x - 100 && mouseX < position.x - 100 + width && mouseY > position.y - 150 && mouseY < position.y -150 + height) {
				app->render->DrawTexture(texture3, position.x, position.y - 100);
				if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
					npcClick = true;
					//sale dialogo
					app->dialogManager->StartDialog(1);
				}
			}
		}
		else {
			if (touchingNpc) {
				app->render->DrawTexture(texture2, position.x, position.y - 100);
				if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
					//sale dialogo
					app->dialogManager->StartDialog(1);
				}
			}
		}
		app->render->DrawTexture(anim.texture, position.x - 100, position.y - 150, &anim.GetCurrentFrame());
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