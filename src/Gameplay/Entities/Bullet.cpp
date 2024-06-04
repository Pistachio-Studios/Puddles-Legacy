#include "Core/App.h"
#include "Core/Physics.h"
#include "Gameplay/Entities/Bullet.h"
#include "Core/Render.h"
#include "Core/Textures.h"
#include "Utils/Log.h"
#include "Utils/Timer.h"

#include <imgui.h>

Bullet::Bullet() : Entity(EntityType::BULLET)
{
	name.Create("Bullet");
}

Bullet::~Bullet() {
}

bool Bullet::Awake() {

	return true;
}

bool Bullet::Start() {

	pbody = app->physics->CreateRectangle(position.x, position.y, 90, 20, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::BULLET;
	texture = app->tex->Load("Assets/Textures/bullet.png");
	timer = Timer();

	position = { 0,0 };

	return true;
}

bool Bullet::Update(float dt)
{
	if (active) {
		//pbody->GetPosition(position.x, position.y);
		app->render->DrawTexture(texture, position.x, position.y, 0, 1.0f, pbody->body->GetAngle() * RADTODEG);
	}

	if (timer.ReadSec() > 1) {
		active = false;
	}

	return true;
}


void Bullet::DrawImGui()
{
}

void Bullet::Shoot()
{
	active = true;
	timer.Start();
	pbody->body->SetAwake(false);
	pbody->body->SetAwake(true);
	pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0);
}

bool Bullet::CleanUp() {

	app->physics->DestroyBody(pbody);
	app->tex->UnLoad(texture);

	return true;
}

void Bullet::OnCollision(PhysBody* physA, PhysBody* physB) {

}

void Bullet::EndCollision(PhysBody* physA, PhysBody* physB) {

}