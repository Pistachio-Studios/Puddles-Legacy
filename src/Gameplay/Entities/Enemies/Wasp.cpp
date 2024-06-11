#include "Gameplay/Entities/Enemies/Wasp.h"
#include "Core/App.h"
#include "Gameplay/Entities/Entity.h"
#include "Gameplay/Entities/Player.h"
#include "Core/Textures.h"
#include "Core/Input.h"
#include "Core/Render.h"
#include "Gameplay/Scene.h"
#include "Utils/Point.h"
#include "Core/Physics.h"
#include "Core/Audio.h"
#include "Utils/StateMachine.h"
#include "Core/SceneManager.h"
#include "Core/Map.h"
#include "Core/AnimationManager.h"


#include "Gameplay/States/Wasp/WaspIdleState.hpp"
#include "Gameplay/States/Wasp/WaspAttackState.hpp"
#include "Gameplay/States/Wasp/WaspMoveState.hpp"
#include "Gameplay/States/Wasp/WaspHurtState.hpp"
#include "Gameplay/States/Wasp/WaspDeadState.hpp"
#include "Gameplay/States/Wasp/WaspParalyzedState.hpp"

#include <cmath>
#include <iostream>

#include <box2d/b2_math.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>

#include <imgui.h>

Wasp::Wasp() : Entity(EntityType::WASP)
{
	name.Create("Wasp");
}

Wasp::~Wasp() {

}

bool Wasp::Awake() {
	return true;
}

bool Wasp::Start() {

	pathfinding = new PathFinding();

	pathfinding->SetNavigationMap((uint)app->map->mapData.width, (uint)app->map->mapData.height, app->map->navigationMap);

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	newPosition = spawnPosition = position;

	timer = Timer();

	pbody = app->physics->CreateRectangle(position.x, position.y, 64, 64, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMYWASP; 

	//si quieres dar vueltos como la helice de un helicoptero Boeing AH-64 Apache pon en false la siguiente funcion
	pbody->body->SetFixedRotation(true);
	//pbody->body->GetFixtureList()->SetFriction(25.0f);
	pbody->body->SetLinearDamping(10.0f);

	pbody->body->GetFixtureList()->SetSensor(true);

	player = app->entityManager->GetPlayerEntity();

	movementFSM = new StateMachine<Wasp>(this);
	movementFSM->AddState(new WaspIdleState("idle"));
	movementFSM->AddState(new WaspMoveState("move"));
	movementFSM->AddState(new WaspHurtState("hurt"));
	movementFSM->AddState(new WaspAttackState("attack"));
	movementFSM->AddState(new WaspDeadState("die"));
	movementFSM->AddState(new WaspParalyzedState("paralyzed"));

	//Animations
	waspIdle = *app->animationManager->GetAnimByName("Avispa_Idle");
	waspIdle.speed = 2.0f;

	waspAttack = *app->animationManager->GetAnimByName("Avispa_Atacar");
	waspAttack.speed = 2.0f;

	waspMove = *app->animationManager->GetAnimByName("Avispa_Volar");
	waspMove.speed = 2.0f;	
	
	waspDamage = *app->animationManager->GetAnimByName("Avispa_Damage");
	waspDamage.speed = 2.0f;

	waspDeath = *app->animationManager->GetAnimByName("Avispa_Muerte");
	waspDeath.speed = 2.0f;

	damageFx = app->audio->LoadFx(parameters.attribute("damageFxPath").as_string());
	attackFx = app->audio->LoadFx(parameters.attribute("attackFxPath").as_string());
	dieFx = app->audio->LoadFx(parameters.attribute("dieFxPath").as_string());

	damage = new ParticleGenerator();
	damage->emiting = false;
	damage->oneShoot = true;
	damage->lifetime = 0.25f;
	damage->explosiveness = 1.0f;
	damage->spawnRadius = 50;
	damage->size = 30;
	damage->initialVelocity = 0;
	damage->Damping = 0.0f;
	damage->spread = 180;
	damage->sizeFade = -1.0f;
	damage->opacityFade = 0.5f;
	damage->color = { 128, 128, 0, 128 };
	app->particleManager->AddGenerator(damage);

	paralyzedParticles = new ParticleGenerator();
	paralyzedParticles->emiting = false;
	paralyzedParticles->oneShoot = true;
	paralyzedParticles->lifetime = 0.25f;
	paralyzedParticles->explosiveness = 1.0f;
	paralyzedParticles->spawnRadius = 50;
	paralyzedParticles->size = 30;
	paralyzedParticles->initialVelocity = 0;
	paralyzedParticles->Damping = 0.0f;
	paralyzedParticles->spread = 180;
	paralyzedParticles->sizeFade = -1.0f;
	paralyzedParticles->opacityFade = 0.5f;
	paralyzedParticles->color = { 50, 128, 128, 255 };
	app->particleManager->AddGenerator(paralyzedParticles);

	return true;
}

bool Wasp::Update(float dt)
{
	movementFSM->Update(dt);
	pbody->body->SetTransform(pbody->body->GetPosition(), 0);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	damage->position = { position.x + 16, position.y + 16 };

	//app->render->DrawRectangle({ position.x - 1, position.y + 8, 36, 18 }, 255, 0, 0);

	app->render->DrawLine(METERS_TO_PIXELS(pbody->body->GetPosition().x), METERS_TO_PIXELS(pbody->body->GetPosition().y), METERS_TO_PIXELS(pbody->body->GetPosition().x) + pbody->body->GetLinearVelocity().x * 10, METERS_TO_PIXELS(pbody->body->GetPosition().y) + +pbody->body->GetLinearVelocity().y * 10, 255, 255, 0);


	/* 	app->render->DrawTexture(currentAnimation->texture, position.x - 9, position.y - 9, &currentAnimation->GetCurrentFrame(), 1.0f, pbody->body->GetAngle()*RADTODEG, flip);

		currentAnimation->Update(dt); */

	// Calculate the angle between the enemy and the player
	
	float angleToPlayer = atan2(player->position.y - position.y, player->position.x - position.x);

	// Determine if the player is to the left or right of the enemy
	if (player->position.x < position.x) {
		flip = SDL_FLIP_NONE;
	}
	else {
		flip = SDL_FLIP_HORIZONTAL;
	}

	return true;
}

void Wasp::DrawImGui()
{
	ImGui::Begin("Enemy");
	ImGui::Text("Enemy Position: %d, %d", position.x, position.y);
	ImGui::Text("Enemy Speed: %f", pbody->body->GetLinearVelocity().Length());
	ImGui::SliderFloat("max speed", &maxSpeed, 1.0f, 10.0f);
	ImGui::SliderFloat("move force", &moveForce, 1.0f, 10.0f);
	ImGui::End();
}

void Wasp::Idle(float dt) {

}

void Wasp::Move(float dt) {
	// TODO move logic
}

void Wasp::Attack(float dt)
{

}

bool Wasp::SaveState(pugi::xml_node& node) {

	pugi::xml_node WaspAttributes = node.append_child("enemies").append_child("Wasp");
	WaspAttributes.append_attribute("x").set_value(this->position.x);
	WaspAttributes.append_attribute("y").set_value(this->position.y);

	return true;
}

bool Wasp::LoadState(pugi::xml_node& node)
{
	pbody->body->SetTransform({ PIXEL_TO_METERS(node.child("enemies").child("Wasp").attribute("x").as_int()), PIXEL_TO_METERS(node.child("enemies").child("Wasp").attribute("y").as_int())}, node.child("enemies").child("Wasp").attribute("angle").as_int());
	// reset player physics
	pbody->body->SetAwake(false);
	pbody->body->SetAwake(true);

	return true;
}

void Wasp::StopMoving() 
{
	if (PIXEL_TO_METERS(player->position.DistanceTo(position)) > 5.0f) { updateSpeed == noSpeed; }
	else { updateSpeed == moveSpeed; }
	
	pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y)}, 0); 

	pbody->body->SetAwake(false);
	pbody->body->SetAwake(true);
}

void Wasp::moveToSpawnPoint()
{
	position = spawnPosition;

	pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0);

	pbody->body->SetAwake(false);
	pbody->body->SetAwake(true);
}

bool Wasp::CleanUp() {

	app->tex->UnLoad(texture);
	app->physics->DestroyBody(pbody);

	return true;
}

void Wasp::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype) {

	case ColliderType::SWORD:
		LOG("Collision SWORD");
		//if (state != EntityState::DEAD and !invencible){
		vida -= player->strength - (defense / 2);
		damage->emiting = true;
		if (vida <= 0.0f && !dead)
		{
			dead = true;
			app->audio->PlayFx(dieFx);
			player->bestiary->enemy1Killed = true;
			movementFSM->ChangeState("die");
		}
		else if (vida > 0.0f) {
			app->audio->PlayFx(damageFx);
			waspDamage.Reset();

			if (player->stealLife) {
				player->vida += player->stealLifeRatio;
				LOG("Player steal life %f", player->vida);
			}

			if (app->entityManager->GetPlayerEntity()->bleed) {
				// 15% change to bleed
				if (rand() % 100 < player->bleedChance) {
					// TODO add bleed effect
					vida -= 1.0f;
					LOG("Wasp Bleed! %f", vida);
				}
			}

			movementFSM->ChangeState("hurt");
		}
		//else {
		//	// AUDIO DONE boss hit
		//	app->audio->PlayFx(bossHit);
		//	movementStateMachine->ChangeState("hurt");
		//	lives--;
		//}
		break;

	case ColliderType::PLAYER:
		isTouchingPlayer = true;
		break;

	case ColliderType::MAGIC:
		vida -= player->intelligence - (defense / 2);
		damage->emiting = true;
		if (vida <= 0.0f)
		{
			// AUDIO DONE boss death
			app->audio->PlayFx(dieFx);
			player->bestiary->enemy1Killed = true;
			movementFSM->ChangeState("die");
		}
		else if (vida > 0.0f) {
			app->audio->PlayFx(damageFx);
			waspDamage.Reset();
			movementFSM->ChangeState("hurt");
		}
		break;

	case ColliderType::UNKNOWN:
		LOG("Colision UNKNOWN");
		break;
	}
}

void Wasp::EndCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype) {
	case ColliderType::PLAYER:
		isTouchingPlayer = false;
		break;
	}
}

b2Vec2 Wasp::calculateForce() {
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
	float attackForce = 10.0f; // Adjust this value as needed

	// Apply the impulse/force towards the player
	b2Vec2 force(dirX * attackForce, dirY * attackForce);

	return force;
}

void Wasp::attackMovement(b2Vec2 force) {
	
	pbody->body->ApplyLinearImpulse(force, pbody->body->GetWorldCenter(), true);
}

void Wasp::pathfindingMovement(float dt) {

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

void Wasp::OnRaycastHit(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {

}
