#include "Core/App.h"
#include "Gameplay/Entities/Sword.h"
#include "Core/Render.h"

#include <imgui.h>

Sword::Sword() : Entity(EntityType::PLAYER)
{
	name.Create("Sword");
}

Sword::~Sword() {

}

bool Sword::Awake() {

	return true;
}

bool Sword::Start() {
	pbody = app->physics->CreateRectangle(position.x, position.y, 30, 10, bodyType::KINEMATIC);
	return true;
}

bool Sword::Update(float dt)
{
	pbody->GetPosition(position.x, position.y);
	//app->render->DrawRectangle({ position.x, position.y, 30, 10 }, 255, 0, 0);
	return true;
}

void Sword::DrawImGui()
{
}

bool Sword::CleanUp() {
	app->physics->DestroyBody(pbody);
	return true;
}

void Sword::OnCollision(PhysBody* physA, PhysBody* physB) {

}

void Sword::EndCollision(PhysBody* physA, PhysBody* physB) {

}