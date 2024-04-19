#include "Gameplay/Entities/Npcs/Npc.h"
#include "Core/App.h"
#include "Gameplay/Entities/Entity.h"
#include "Core/Textures.h"
#include "Core/Input.h"
#include "Core/Render.h"
#include "Gameplay/Scene.h"
#include "Utils/Point.h"
#include "Utils/Log.h"

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
	texturePath = parameters.attribute("texturepath").as_string();
	texturePath2 = parameters.attribute("texturepath2").as_string();

	texture = app->tex->Load(texturePath);
	texture2 = app->tex->Load(texturePath2);

	pbody = app->physics->CreateRectangle(position.x, position.y, 64, 128, bodyType::STATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::NPC;

	return true;
}

bool Npc::Update(float dt)
{
	pbody->body->SetTransform(pbody->body->GetPosition(), 0);

	int width, height;
	SDL_QueryTexture(texture, NULL, NULL, &width, &height);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - width / 2;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - height / 2;

	app->render->DrawTexture(texture, position.x, position.y);

	if (touchingNpc) {
		app->render->DrawTexture(texture2, position.x, position.y);
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

	app->tex->UnLoad(texture);
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