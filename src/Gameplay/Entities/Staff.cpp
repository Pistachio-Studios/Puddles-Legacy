#include "Core/App.h"
#include "Gameplay/Entities/Staff.h"
#include "Core/Physics.h"
#include "Core/Render.h"
#include "Core/Textures.h"
#include "Utils/Log.h"

#include <imgui.h>

Magic::Magic() {
	texture = app->tex->Load("Assets/Textures/sword.png");
	pbody = app->physics->CreateCircle(0, 0, 10, bodyType::DYNAMIC);
	timer = new Timer();
}

void Magic::Throw() {
	active = true;
	timer->Start();
	pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0);

	b2Vec2 mouseWorldPosition = { PIXEL_TO_METERS(app->input->GetMouseX()) + PIXEL_TO_METERS(-app->render->camera.x), PIXEL_TO_METERS(app->input->GetMouseY()) + PIXEL_TO_METERS(-app->render->camera.y) };

	b2Vec2 lookingDir = mouseWorldPosition - pbody->body->GetPosition();
	lookingDir.Normalize();
	pbody->body->ApplyForceToCenter(b2Vec2(lookingDir.x * 10, lookingDir.y * 10), true);
}

void Magic::Draw() {
	if (active) {
		pbody->GetPosition(position.x, position.y);
		app->render->DrawTexture(texture, position.x, position.y, 0, 1.0f, pbody->body->GetAngle()*RADTODEG);
	}
}

void Magic::Update() {
	Draw();
	if (timer->ReadSec() > 1) {
		active = false;
	}
}

void Magic::CleanUp() {
	app->physics->DestroyBody(pbody);
	app->tex->UnLoad(texture);
}

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
	texture = app->tex->Load("Assets/Textures/sword.png");
	pbody = app->physics->CreateRectangle(position.x, position.y, 90, 20, bodyType::KINEMATIC);
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
	return true;
}

void Staff::OnCollision(PhysBody* physA, PhysBody* physB) {

}

void Staff::EndCollision(PhysBody* physA, PhysBody* physB) {

}

void Staff::ThrowSpell() {
	magic[magicIndex].position = { position.x, position.y};
	magic[magicIndex].Throw();
	magicIndex++;
	if (magicIndex >= 10) {
		magicIndex = 0;
	}
}