#include "Core/App.h"
#include "Gameplay/Entities/Sword.h"
#include "Core/Render.h"
#include "Core/Textures.h"
#include "Utils/Log.h"

#include <imgui.h>

Sword::Sword() : Entity(EntityType::PLAYER)
{
	name.Create("Sword");

	pbody = app->physics->CreateRectangle(position.x, position.y, 90, 20, bodyType::KINEMATIC);
	texture = app->tex->Load("Assets/Textures/espadatest.png");
}

Sword::~Sword() {
}

bool Sword::Update(float dt)
{
	if(active)
	{
		pbody->GetPosition(position.x, position.y);
		app->render->DrawTexture(texture, position.x + 35, position.y - 35, 0, 1.0f, pbody->body->GetAngle()*RADTODEG+90);
	}
	return true;
}

void Sword::Equip()
{
	active = true;
	pbody = app->physics->CreateRectangle(position.x, position.y, 90, 20, bodyType::KINEMATIC);
	pbody->ctype = ColliderType::SWORD;
}

void Sword::Store()
{
	active = false;
	app->physics->DestroyBody(pbody);
	position = { 0,0 };
}

void Sword::DrawImGui()
{
}

bool Sword::CleanUp() {
	app->physics->DestroyBody(pbody);
	app->tex->UnLoad(texture);
	return true;
}

void Sword::OnCollision(PhysBody* physA, PhysBody* physB) {

}

void Sword::EndCollision(PhysBody* physA, PhysBody* physB) {

}