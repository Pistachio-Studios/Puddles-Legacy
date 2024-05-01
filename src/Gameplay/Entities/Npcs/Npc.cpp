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
  
	texture2 = app->tex->Load("Assets/Textures/pressE.png");
	texture3 = app->tex->Load("Assets/Textures/click.png");

	pbody = app->physics->CreateRectangle(position.x, position.y, 42, 65, bodyType::KINEMATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::NPC;

	npcSensor = app->physics->CreateRectangleSensor(position.x, position.y, 128, 128, bodyType::STATIC);
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

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - width / 2;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - height / 2;

	app->render->DrawTexture(anim.texture, position.x, position.y, &anim.GetCurrentFrame());
	anim.Update(dt);


	if (app->sceneManager->GetCurrentScene()->name == "tutorialscene")  { // TODO change this if
		int mouseX = METERS_TO_PIXELS(mouseWorldPosition.x);
		int mouseY = METERS_TO_PIXELS(mouseWorldPosition.y);
		if (mouseX > position.x && mouseX < position.x + width && mouseY > position.y && mouseY < position.y + height) {
			app->render->DrawTexture(texture3, position.x - 40, position.y - 50);
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
				npcClick = true;
				//sale dialogo
				app->dialogManager->StartDialog(1);
			}
		}
	} else {
		if(touchingNpc)
			app->render->DrawTexture(texture2, position.x - 40, position.y - 20);
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