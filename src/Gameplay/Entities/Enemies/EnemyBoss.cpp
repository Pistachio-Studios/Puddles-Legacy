#include "Core/App.h"
#include "Core/Textures.h"
#include "Core/Input.h"
#include "Core/Render.h"
#include "Core/Physics.h"
#include "Core/Map.h"
#include "Core/AnimationManager.h"
#include "Core/SceneManager.h"
#include "Core/Audio.h"
#include "Gameplay/Entities/Enemies/EnemyBoss.h"
#include "Gameplay/Entities/Entity.h"
#include "Gameplay/Entities/Player.h"
#include "Gameplay/Scene.h"
#include "Gameplay/Entities/Bullet.h"
#include "Utils/Point.h"
#include "Utils/StateMachine.h"
#include "Core/ParticleManager.h"

#include "Gameplay/States/EnemyBoss/EnemyBossIdleState.hpp"
#include "Gameplay/States/EnemyBoss/EnemyBossBodyAttackState.hpp"
#include "Gameplay/States/EnemyBoss/EnemyBossDistanceAttackState.hpp"
#include "Gameplay/States/EnemyBoss/EnemyBossMoveState.hpp"
#include "Gameplay/States/EnemyBoss/EnemyBossHurtState.hpp"
#include "Gameplay/States/EnemyBoss/EnemyBossDeadState.hpp"

#include <cmath>
#include <iostream>

#include <box2d/b2_math.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>

#include <imgui.h>

EnemyBoss::EnemyBoss() : Entity(EntityType::ENEMYBOSS)
{
	name.Create("EnemyBoss");
}

EnemyBoss::~EnemyBoss() {

}

bool EnemyBoss::Awake() {

	return true;
}

bool EnemyBoss::Start() {


	pathfinding = new PathFinding();

	pathfinding->SetNavigationMap((uint)app->map->mapData.width, (uint)app->map->mapData.height, app->map->navigationMap);

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string(); 
	newPosition = spawnPosition = position;

	timer = Timer();

	pbody = app->physics->CreateRectangle(position.x, position.y, 230, 192, bodyType::DYNAMIC); 
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMYBOSS; 

	//si quieres dar vueltos como la helice de un helicoptero Boeing AH-64 Apache pon en false la siguiente funcion
	pbody->body->SetFixedRotation(true);
	//pbody->body->GetFixtureList()->SetFriction(25.0f);
	pbody->body->SetLinearDamping(10.0f);

	player = app->entityManager->GetPlayerEntity();

	movementFSM = new StateMachine<EnemyBoss>(this);
	movementFSM->AddState(new EnemyBossIdleState("idle"));
	movementFSM->AddState(new EnemyBossMoveState("move"));
	movementFSM->AddState(new EnemyBossHurtState("hurt"));
	movementFSM->AddState(new EnemyBossBodyAttackState("bodyAttack"));
	movementFSM->AddState(new EnemyBossDistanceAttackState("distanceAttack"));
	movementFSM->AddState(new EnemyBossDeadState("die"));

	//Animations
	bossIdle = *app->animationManager->GetAnimByName("Boss_Spider_Idle");
	bossIdle.speed = 2.0f;

	bossDistanceAttack = *app->animationManager->GetAnimByName("Boss_Spider_Ataque_distancia");
	bossDistanceAttack.speed = 1.0f;

	bossBodyAttack = *app->animationManager->GetAnimByName("Boss_Spider_Ataque_cuerpo");
	bossBodyAttack.speed = 2.0f;

	bossMove = *app->animationManager->GetAnimByName("Boss_Spider_Caminar");
	bossMove.speed = 1.4f;

	bossDamage = *app->animationManager->GetAnimByName("Boss_Spider_Damage");
	bossDamage.speed = 1.0f;

	bossDeath = *app->animationManager->GetAnimByName("Boss_Spider_Muerte");
	bossDeath.speed = 1.0f;

	//Attack
	for (int i = 0; i < 10; i++) {
		bulletArray[i] = new Bullet();
	}

	bossDamageFx = app->audio->LoadFx(parameters.attribute("BossDamagePath").as_string());
	bossAttackFx = app->audio->LoadFx(parameters.attribute("BossAttackPath").as_string());
	bossDieFx = app->audio->LoadFx(parameters.attribute("BossDiePath").as_string());

	damage = new ParticleGenerator();
	damage->emiting = false;
	damage->oneShoot = true;
	damage->lifetime = 0.25f;
	damage->explosiveness = 1.0f;
	damage->spawnRadius = 90;
	damage->size = 30;
	damage->initialVelocity = 0;
	damage->Damping = 0.0f;
	damage->spread = 180;
	damage->sizeFade = -1.0f;
	damage->opacityFade = 0.5f;
	damage->color = { 0, 200, 100, 128 };
	app->particleManager->AddGenerator(damage);

	return true;
}

bool EnemyBoss::Update(float dt)
{
	
	movementFSM->Update(dt);
	pbody->body->SetTransform(pbody->body->GetPosition(), 0);

	app->render->DrawLine(METERS_TO_PIXELS(pbody->body->GetPosition().x), METERS_TO_PIXELS(pbody->body->GetPosition().y), METERS_TO_PIXELS(pbody->body->GetPosition().x) + pbody->body->GetLinearVelocity().x * 7, METERS_TO_PIXELS(pbody->body->GetPosition().y) + +pbody->body->GetLinearVelocity().y * 7, 255, 255, 0);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	damage->position = { position.x + 16, position.y + 16 };

	/* 	app->render->DrawTexture(currentAnimation->texture, position.x - 9, position.y - 9, &currentAnimation->GetCurrentFrame(), 1.0f, pbody->body->GetAngle()*RADTODEG, flip);

		currentAnimation->Update(dt); */

	float angleToPlayer = atan2(player->position.y - position.y, player->position.x - position.x);

	// Determine if the player is to the left or right of the enemy
	if (player->position.x < position.x) {
		flip = SDL_FLIP_HORIZONTAL;
	}
	else {
		flip = SDL_FLIP_NONE;
	}

	if (active)
	{
		pbody->GetPosition(position.x, position.y);
		app->render->DrawTexture(texture, position.x + 35, position.y - 35, 0, 1.0f, pbody->body->GetAngle() * RADTODEG + 90);
		for (int i = 0; i < 10; i++) {
			bulletArray[i]->Update(CalculateAngleToPlayer());
		}
	}

	return true;
}

void EnemyBoss::DrawImGui()
{
	ImGui::Begin("Enemy");
	ImGui::Text("Enemy Position: %d, %d", position.x, position.y);
	ImGui::Text("Enemy Speed: %f", pbody->body->GetLinearVelocity().Length());
	ImGui::SliderFloat("max speed", &maxSpeed, 1.0f, 10.0f);
	ImGui::SliderFloat("move force", &moveForce, 1.0f, 10.0f);
	for (int i = 0; i < 10; i++) {
		ImGui::Text("Magic %d", i);
		ImGui::Text("Active: %s", bulletArray[i]->active ? "true" : "false");
		ImGui::Text("Position: %d, %d", bulletArray[i]->position.x, bulletArray[i]->position.y);

		ImGui::Separator();
	}

	ImGui::End();
}

void EnemyBoss::Idle(float dt) {

}

void EnemyBoss::Move(float dt) {
	// TODO move logic
}

void EnemyBoss::Attack(float dt)
{

}

bool EnemyBoss::SaveState(pugi::xml_node& node) {

	pugi::xml_node enemybossAttributes = node.append_child("enemies").append_child("EnemyBoss");
	enemybossAttributes.append_attribute("x").set_value(this->position.x);
	enemybossAttributes.append_attribute("y").set_value(this->position.y);

	return true;
}

bool EnemyBoss::LoadState(pugi::xml_node& node)
{
	pbody->body->SetTransform({ PIXEL_TO_METERS(node.child("enemies").child("EnemyBoss").attribute("x").as_int()), PIXEL_TO_METERS(node.child("enemies").child("EnemyBoss").attribute("y").as_int())}, node.child("enemies").child("EnemyBoss").attribute("angle").as_int());
	// reset player physics
	pbody->body->SetAwake(false);
	pbody->body->SetAwake(true);

	return true;
}

void EnemyBoss::StopMoving() 
{
	if (PIXEL_TO_METERS(player->position.DistanceTo(position)) > 5.0f) { updateSpeed == noSpeed; }
	else { updateSpeed == moveSpeed; }
	
	pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)}, 0); 

	pbody->body->SetAwake(false);
	pbody->body->SetAwake(true);
}

void EnemyBoss::moveToSpawnPoint()
{
	position = spawnPosition;

	pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0);

	pbody->body->SetAwake(false);
	pbody->body->SetAwake(true);
}

bool EnemyBoss::CleanUp() {

	app->tex->UnLoad(texture);
	app->physics->DestroyBody(pbody);

	return true;
}

void EnemyBoss::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype) {
	
	case ColliderType::SWORD:
		LOG("Collision ARMAPLAYER");
		//if (state != EntityState::DEAD and !invencible){
		vida -= player->strength - (defense / 2);
		damage->emiting = true;
		if (vida <= 0.0f && !dead)
		{
			dead = true;
			app->audio->PlayFx(bossDieFx);
			movementFSM->ChangeState("die");
		}
		else if(vida > 0.0f)
		{
			app->audio->PlayFx(bossDamageFx);
			bossDamage.Reset();
			movementFSM->ChangeState("hurt");
		}
		break;

	case ColliderType::MAGIC:
		vida -= player->intelligence - (defense / 2);
		damage->emiting = true;
		if (vida <= 0.0f && !dead)
		{
			dead = true;
			app->audio->PlayFx(bossDieFx);
			movementFSM->ChangeState("die");
		}
		else if (vida > 0.0f) {
			app->audio->PlayFx(bossDamageFx);
			bossDamage.Reset();
			movementFSM->ChangeState("hurt");
		}
		break;

	case ColliderType::PLAYER:
		isTouchingPlayer = true;
		break;

	case ColliderType::UNKNOWN:
		LOG("Colision UNKNOWN");
		break;
	}
}

void EnemyBoss::EndCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype) {
	case ColliderType::PLAYER:
		isTouchingPlayer = false;
		break;
	case ColliderType::UNKNOWN:
		LOG("Colision UNKNOWN");
		break;
	}
}

void EnemyBoss::pathfindingMovement(float dt) {

	iPoint origin = app->map->WorldToMap(newPosition.x, newPosition.y); //a�adir el tile size / 2 hace que el owl se acerque mas 

	if (timer.ReadMSec() > 250) {
		iPoint destination = app->map->WorldToMap(player->position.x, player->position.y);  //a�adir el tile size / 2 hace que el owl se acerque mas
		pathfinding->CreatePath(origin, destination); 
		timer.Start();
		currentPathPos = 0;
	}

	const DynArray<iPoint>* path = pathfinding->GetLastPath();

	if (movementDelay.ReadMSec() > 100) {
		if (currentPathPos < path->Count())
		{
			newPosition = app->map->MapToWorld(path->At(currentPathPos)->x, path->At(currentPathPos)->y);
			currentPathPos++;
			//LOG("%d", currentPathPos);
			movementDelay.Start();
		}
	}

	pbody->body->SetTransform( 
		{
			std::lerp(pbody->body->GetPosition().x, PIXEL_TO_METERS(newPosition.x), dt * moveSpeed / 1000), 
			std::lerp(pbody->body->GetPosition().y, PIXEL_TO_METERS(newPosition.y), dt * moveSpeed / 1000) 

		},

		angle * DEGTORAD
	);

	if (debug)
	{
		//Render Path
		if (path->Count() > 0)
		{
			for (uint i = 0; i < path->Count() - 1; ++i)
			{
				iPoint pos1 = app->map->MapToWorld(path->At(i)->x, path->At(i)->y);
				iPoint pos2 = app->map->MapToWorld(path->At(i + 1)->x, path->At(i + 1)->y);
				app->render->DrawLine(pos1.x, pos1.y, pos2.x, pos2.y, 0, 0, 255);
			}

		}
	}
}

void EnemyBoss::shootBullet()
{
	for (int i = 0; i < 10; i++) {
		if (!bulletArray[i]->active) {
			bulletArray[i]->position = position;
			bulletArray[i]->Shoot(calculateForce(),flip);
			break;
		}
	}
}

float EnemyBoss::CalculateAngleToPlayer()
{
	if (justShot) {

		// Calculate the difference in positions
		float dx = player->position.x - position.x;
		float dy = player->position.y - position.y;

		// Use atan2 to get the angle in radians
		float angle = atan2(dy, dx);

		// Optionally convert the angle to degrees
		angleDegrees = angle * (180.0f / M_PI);
		justShot = false;
	}

	return angleDegrees;
}

b2Vec2 EnemyBoss::calculateForce()
{
	// Calculate the direction vector from the enemy to the player
	float dirX = player->position.x - position.x;
	float dirY = player->position.y - position.y;

	// Normalize the direction vector
	float length = sqrt(dirX * dirX + dirY * dirY);
	if (length != 0) {
		dirX /= length;
		dirY /= length;
	}

	// Define the attack force or speed
	float attackForce = 1.2f; // Adjust this value as needed

	// Apply the impulse/force towards the player
	b2Vec2 force(dirX * attackForce, dirY * attackForce);

	return force;
}

void EnemyBoss::OnRaycastHit(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {

}
