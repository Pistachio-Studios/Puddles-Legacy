#include "Core/App.h"
#include "Gameplay/Entities/Bullet.h"
#include "Core/Physics.h"
#include "Core/Render.h"
#include "Core/Textures.h"
#include "Utils/Log.h"

#include <imgui.h>

Bullet::Bullet() {
	texture = app->tex->Load("Assets/Textures/Bullet.png");
	pbody = app->physics->CreateCircle(0, 0, 10, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::BULLET;
	timer = new Timer();
}

void Bullet::Shoot(b2Vec2 force) {
	active = true;
	timer->Start();

	//resets the inertia of the body
	pbody->body->SetAwake(false);
	pbody->body->SetAwake(true);
	pbody->body->GetFixtureList()->SetSensor(true);
	pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)), 0);

	pbody->body->ApplyForceToCenter(b2Vec2(force.x * 100, force.y * 100), true);
}

void Bullet::Draw() {
	if (active) {
		pbody->GetPosition(position.x, position.y);
		app->render->DrawTexture(texture, position.x, position.y, 0, 1.0f, pbody->body->GetAngle() * RADTODEG,0.06f);
	}
}

void Bullet::Update() {
	Draw();
	if (timer->ReadSec() > 1) {
		active = false;
	}
}

void Bullet::CleanUp() {
	app->physics->DestroyBody(pbody);
	app->tex->UnLoad(texture);
}