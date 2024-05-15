#include "Gameplay/Entities/Enemies/FlyingEnemy.h"
#include "Core/App.h"
#include "Gameplay/Entities/Entity.h"
#include "Gameplay/Entities/Player.h"
#include "Core/Textures.h"
#include "Core/Input.h"
#include "Core/Render.h"
#include "Gameplay/Scene.h"
#include "Utils/Point.h"
#include "Core/Physics.h"
#include "Utils/StateMachine.h"
#include "Core/SceneManager.h"
#include "Core/Map.h"


#include "Gameplay/States/FlyingEnemy/FlyingEnemyIdleState.hpp"
#include "Gameplay/States/FlyingEnemy/FlyingEnemyAttackState.hpp"
#include "Gameplay/States/FlyingEnemy/FlyingEnemyMoveState.hpp"
#include "Gameplay/States/FlyingEnemy/FlyingEnemyHurtState.hpp"
#include "Gameplay/States/FlyingEnemy/FlyingEnemyDeadState.hpp"

#include <cmath>
#include <iostream>

#include <box2d/b2_math.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>

#include <imgui.h>

FlyingEnemy::FlyingEnemy() : Entity(EntityType::FLYINGENEMY)
{
	name.Create("FlyingEnemy");
}

FlyingEnemy::~FlyingEnemy() {

}

bool FlyingEnemy::Awake() {

	return true;
}

bool FlyingEnemy::Start() {

	pathfinding = new PathFinding();

	pathfinding->SetNavigationMap((uint)app->map->mapData.width, (uint)app->map->mapData.height, app->map->navigationMap);

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	newPosition = spawnPosition = position;

	timer = Timer();

	pbody = app->physics->CreateRectangle(position.x, position.y, 36, 36, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;

	//si quieres dar vueltos como la helice de un helicoptero Boeing AH-64 Apache pon en false la siguiente funcion
	pbody->body->SetFixedRotation(true);
	//pbody->body->GetFixtureList()->SetFriction(25.0f);
	pbody->body->SetLinearDamping(10.0f);

	player = app->entityManager->GetPlayerEntity();

	movementFSM = new StateMachine<FlyingEnemy>(this);
	movementFSM->AddState(new FlyingEnemyIdleState("idle"));
	movementFSM->AddState(new FlyingEnemyMoveState("move"));
	movementFSM->AddState(new FlyingEnemyHurtState("hurt"));
	movementFSM->AddState(new FlyingEnemyAttackState("attack"));
	movementFSM->AddState(new FlyingEnemyDeadState("die"));

	return true;
}

bool FlyingEnemy::Update(float dt)
{

	if (movementFSM->GetCurrentState().name != "die") {
		movementFSM->Update(dt);
		pbody->body->SetTransform(pbody->body->GetPosition(), 0);

		app->render->DrawLine(METERS_TO_PIXELS(pbody->body->GetPosition().x), METERS_TO_PIXELS(pbody->body->GetPosition().y), METERS_TO_PIXELS(pbody->body->GetPosition().x) + pbody->body->GetLinearVelocity().x * 10, METERS_TO_PIXELS(pbody->body->GetPosition().y) + +pbody->body->GetLinearVelocity().y * 10, 255, 255, 0);

		//Update player position in pixels
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

		if (debug) {
			if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN) {
				freeCam = !freeCam;
			}
		}

		app->render->DrawRectangle({ position.x - 1, position.y - 2, 36, 36 }, 0, 50, 255);

		app->render->DrawCircle(position.x, position.y + 5, 1, 0, 0, 0, 0);
		/* 	app->render->DrawTexture(currentAnimation->texture, position.x - 9, position.y - 9, &currentAnimation->GetCurrentFrame(), 1.0f, pbody->body->GetAngle()*RADTODEG, flip);

			currentAnimation->Update(dt); */

			// Calculate the angle between the enemy and the player
		float angleToPlayer = atan2(player->position.y - position.y, player->position.x - position.x);
	}

	

	return true;
}

void FlyingEnemy::DrawImGui()
{
	ImGui::Begin("FlyingEnemy");
	ImGui::Text("FlyingEnemy Position: %d, %d", position.x, position.y);
	ImGui::Text("FlyingEnemy Speed: %f", pbody->body->GetLinearVelocity().Length());
	ImGui::SliderFloat("max speed", &maxSpeed, 1.0f, 10.0f);
	ImGui::SliderFloat("move force", &moveForce, 1.0f, 10.0f);
	ImGui::End();
}

void FlyingEnemy::Idle(float dt) {

}

void FlyingEnemy::Move(float dt) {
	// TODO move logic
}

void FlyingEnemy::Attack(float dt)
{

}

bool FlyingEnemy::SaveState(pugi::xml_node& node) {

	pugi::xml_node enemybossAttributes = node.append_child("enemies").append_child("FlyingEnemy");
	enemybossAttributes.append_attribute("x").set_value(this->position.x);
	enemybossAttributes.append_attribute("y").set_value(this->position.y);

	return true;
}

bool FlyingEnemy::LoadState(pugi::xml_node& node)
{
	pbody->body->SetTransform({ PIXEL_TO_METERS(node.child("enemies").child("FlyingEnemy").attribute("x").as_int()), PIXEL_TO_METERS(node.child("enemies").child("FlyingEnemy").attribute("y").as_int()) }, node.child("enemies").child("FlyingEnemy").attribute("angle").as_int());
	// reset player physics
	pbody->body->SetAwake(false);
	pbody->body->SetAwake(true);

	return true;
}

void FlyingEnemy::StopMoving()
{
	if (PIXEL_TO_METERS(player->position.DistanceTo(position)) > 5.0f) { updateSpeed == noSpeed; }
	else { updateSpeed == moveSpeed; }

	pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0);

	pbody->body->SetAwake(false);
	pbody->body->SetAwake(true);
}

void FlyingEnemy::moveToSpawnPoint()
{
	position = spawnPosition;

	pbody->body->SetTransform({ PIXEL_TO_METERS(position.x), PIXEL_TO_METERS(position.y) }, 0);

	pbody->body->SetAwake(false);
	pbody->body->SetAwake(true);
}

bool FlyingEnemy::CleanUp() {

	app->tex->UnLoad(texture);
	app->physics->DestroyBody(pbody);

	return true;
}

void FlyingEnemy::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype) {
	
		case ColliderType::SWORD:
			LOG("Collision ARMAPLAYER");
		 	//if (state != EntityState::DEAD and !invencible){
			vida -= player->dano;
			if (vida <= 0.0f)
			{
				// AUDIO DONE boss death
				movementFSM->ChangeState("die");
				app->physics->DestroyBody(pbody);
			}
				//else {
				//	// AUDIO DONE boss hit
				//	app->audio->PlayFx(bossHit);
				//	movementStateMachine->ChangeState("hurt");
				//	lives--;
				//}
			break;

	case ColliderType::PLAYER:
		player->vida -= dano;
		break;

	case ColliderType::UNKNOWN:
		LOG("Colision UNKNOWN");
		break;
	}
}

void FlyingEnemy::EndCollision(PhysBody* physA, PhysBody* physB) {

}

void FlyingEnemy::pathfindingMovement(float dt) {

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

void FlyingEnemy::OnRaycastHit(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {

}
