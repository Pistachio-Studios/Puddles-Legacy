#include "Core/App.h"
#include "Gameplay/Entities/Sword.h"
#include "Core/Render.h"
#include "Core/Textures.h"
#include "Utils/Log.h"

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
	pbody = app->physics->CreateRectangle(position.x, position.y, 90, 20, bodyType::KINEMATIC);
	texture = app->tex->Load("Assets/Textures/sword.png");
	return true;
}

bool Sword::Update(float dt)
{
	pbody->GetPosition(position.x, position.y);
	app->render->DrawTexture(texture, position.x + 35, position.y - 35, 0, 1.0f, pbody->body->GetAngle()*RADTODEG+90);
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