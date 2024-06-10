#include "Core/App.h"
#include "Gameplay/Entities/Staff.h"
#include "Core/Physics.h"
#include "Core/Render.h"
#include "Core/Textures.h"
#include "Utils/Log.h"

#include <imgui.h>

Magic::Magic() : Entity(EntityType::MAGIC) {
	texture = app->tex->Load("Assets/Textures/magic.png");
	pbody = app->physics->CreateCircle(0, 0, 10, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::MAGIC;
	pbody->listener = this;
	timer = new Timer();
	explosion = new ParticleGenerator();
	explosion->emiting = false;
	explosion->explosiveness = 0.9f;
	explosion->direction = { 0, 0 };
	explosion->spread = 360;
	explosion->size = 25;
	explosion->angleRandomness = 180;
	explosion->sizeFade = -1.0f;
	explosion->opacityFade = -1.0f;
	explosion->color = { 200, 0, 200, 255 };
	explosion->initialVelocity = 5;
	explosion->lifetime = 1.0f;
	explosion->amount = 25;
	explosion->oneShoot = true;
	app->particleManager->AddGenerator(explosion);
}

void Magic::Throw() {
	active = true;
	timer->Start();

	//resets the inertia of the body
	pbody->body->SetAwake(false);
	pbody->body->SetAwake(true);
	
	pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0);

	b2Vec2 mouseWorldPosition = { PIXEL_TO_METERS(app->input->GetMouseX()) + PIXEL_TO_METERS(-app->render->camera.x), PIXEL_TO_METERS(app->input->GetMouseY()) + PIXEL_TO_METERS(-app->render->camera.y) };

	b2Vec2 lookingDir = mouseWorldPosition - pbody->body->GetPosition();
	lookingDir.Normalize();
	pbody->body->ApplyForceToCenter(b2Vec2(lookingDir.x * 100, lookingDir.y * 100), true);
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

void Magic::Delete() {
	app->physics->DestroyBody(pbody);
	app->tex->UnLoad(texture);
}

void Magic::OnCollision(PhysBody* physA, PhysBody* physB) {
	if(!physB->body->GetFixtureList()->IsSensor())
	{
		active = false;
		explosion->position = { position.x, position.y };
		explosion->emiting = true;
	}
}

Staff::Staff() : Entity(EntityType::PLAYER)
{
	name.Create("Staff");

	texture = app->tex->Load("Assets/Textures/cetro.png");
	pbody = app->physics->CreateRectangle(position.x, position.y, 90, 20, bodyType::KINEMATIC);
	pbody->body->GetFixtureList()->SetSensor(true);
}

Staff::~Staff() {

}

bool Staff::Update(float dt)
{
	if(active)
	{
		pbody->GetPosition(position.x, position.y);
		app->render->DrawTexture(texture, position.x + 35*2, position.y - 35*2, 0, 1.0f, pbody->body->GetAngle()*RADTODEG+90);
		for (int i = 0; i < 10; i++) {
			magicArray[i]->Update();
		}
	}
	return true;
}

void Staff::Equip()
{
	active = true;
	pbody = app->physics->CreateRectangle(position.x, position.y, 90, 20, bodyType::KINEMATIC);
	pbody->body->GetFixtureList()->SetSensor(true);
	
	//fill the magic pool
	for (int i = 0; i < 10; i++) {
		magicArray[i] = new Magic();
		magicArray[i]->staff = this;
	}
}

void Staff::Store()
{
	active = false;
	app->physics->DestroyBody(pbody);
	for (int i = 0; i < 10; i++) {
		magicArray[i]->Delete();
	}
	position = { 0,0 };
}

void Staff::DrawImGui()
{
	//draw the pool of magic
	ImGui::Begin("Magic Pool");
	for (int i = 0; i < 10; i++) {
		ImGui::Text("Magic %d", i);
		ImGui::Text("Active: %s", magicArray[i]->active ? "true" : "false");
		ImGui::Text("Position: %d, %d", magicArray[i]->position.x, magicArray[i]->position.y);

		ImGui::Separator();
	}
	ImGui::End();
}

bool Staff::CleanUp() {
	return true;
}

void Staff::OnCollision(PhysBody* physA, PhysBody* physB) {
}

void Staff::EndCollision(PhysBody* physA, PhysBody* physB) {

}

void Staff::ThrowSpell() {
	for (int i = 0; i < 10; i++) {
		if (!magicArray[i]->active) {
			magicArray[i]->position = position;
			magicArray[i]->Throw();
			break;
		}
	}
}