#include "Core/App.h"
#include "Gameplay/Entities/Staff.h"
#include "Core/Render.h"
#include "Core/Textures.h"
#include "Utils/Log.h"

#include <imgui.h>

Staff::Staff() : Entity(EntityType::PLAYER)
{
	name.Create("Staff");
}

Staff::~Staff() {

}

bool Staff::Awake() {

	return true;
}

bool Staff::Start() {
	pbody = app->physics->CreateRectangle(position.x, position.y, 90, 20, bodyType::KINEMATIC);
	texture = app->tex->Load("Assets/Textures/sword.png");
	return true;
}

bool Staff::Update(float dt)
{
	pbody->GetPosition(position.x, position.y);
	app->render->DrawTexture(texture, position.x + 35, position.y - 35, 0, 1.0f, pbody->body->GetAngle()*RADTODEG+90);
	return true;
}

void Staff::DrawImGui()
{
}

bool Staff::CleanUp() {
	app->physics->DestroyBody(pbody);
	return true;
}

void Staff::OnCollision(PhysBody* physA, PhysBody* physB) {

}

void Staff::EndCollision(PhysBody* physA, PhysBody* physB) {

}