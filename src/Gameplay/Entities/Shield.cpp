#include "Core/App.h"
#include "Gameplay/Entities/Shield.h"
#include "Core/Render.h"

#include <imgui.h>

Shield::Shield() : Entity(EntityType::PLAYER)
{
	name.Create("Shield");
}

Shield::~Shield() {

}

bool Shield::Awake() {

	return true;
}

bool Shield::Start() {
	pbody = app->physics->CreateRectangle(position.x, position.y, 10, 30, bodyType::KINEMATIC);
	return true;
}

bool Shield::Update(float dt)
{
	pbody->GetPosition(position.x, position.y);
	//app->render->DrawRectangle({ position.x, position.y, 30, 10 }, 255, 0, 0);
	return true;
}

void Shield::DrawImGui()
{
}

bool Shield::CleanUp() {
	app->physics->DestroyBody(pbody);
	return true;
}

void Shield::OnCollision(PhysBody* physA, PhysBody* physB) {

}

void Shield::EndCollision(PhysBody* physA, PhysBody* physB) {

}