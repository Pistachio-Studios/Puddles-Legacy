#include "Gameplay/Entities/Player.h"
#include "Core/App.h"
#include "Gameplay/Entities/Entity.h"
#include "Core/Textures.h"
#include "Core/Input.h"
#include "Core/Render.h"
#include "Gameplay/Scene.h"
#include "Utils/Point.h"
#include "Core/Physics.h"
#include "Utils/StateMachine.h"
#include "Core/SceneManager.h"
#include "Core/Window.h"
#include "Core/AnimationManager.h"


#include "Gameplay/States/Player/PlayerIdleState.hpp"
#include "Gameplay/States/Player/PlayerMoveState.hpp"
#include "Gameplay/States/Player/PlayerCombatIdleState.hpp"
#include "Gameplay/States/Player/PlayerCombatAttackState.hpp"
#include "Gameplay/States/Player/PlayerCombatBlockState.hpp"

#include <SDL_scancode.h>
#include <cmath>
#include <iostream>

#include <box2d/b2_math.h>
#include <box2d/b2_body.h>
#include <box2d/b2_fixture.h>

#include <imgui.h>

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {
	
	return true;
}

bool Player::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	timer = Timer();

	playerHurtCultdown = Timer();

	dashTimer = Timer();

	//texture = app->tex->Load("Assets/Textures/playerx128-test.png");

	pbody = app->physics->CreateRectangle(position.x, position.y, 64, 128, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	//si quieres dar vueltos como la helice de un helicoptero Boeing AH-64 Apache pon en false la siguiente funcion
	pbody->body->SetFixedRotation(true);
	//pbody->body->GetFixtureList()->SetFriction(25.0f);
	pbody->body->SetLinearDamping(10.0f);

	swordEntity = (Sword*)app->entityManager->CreateEntity(EntityType::SWORD);
	staffEntity = (Staff*)app->entityManager->CreateEntity(EntityType::STAFF);

	movementFSM = new StateMachine<Player>(this);
	movementFSM->AddState(new PlayerIdleState("idle")); 
	movementFSM->AddState(new PlayerMoveState("move"));

	combatFSM = new StateMachine<Player>(this);
	combatFSM->AddState(new PlayerCombatIdleState("idle"));
	combatFSM->AddState(new PlayerCombatAttackState("attack"));
	combatFSM->AddState(new PlayerCombatBlockState("block"));

	//TODO parpadeos movimiento & overlay idle con ataque
	//Anims
	SabrinaEspadaIdle = *app->animationManager->GetAnimByName("SabrinaEspadaIdle_1");
	SabrinaEspadaIdle.speed = 2.0f;

	SabrinaCetroIdle = *app->animationManager->GetAnimByName("SabrinaCetroIdle");
	SabrinaCetroIdle.speed = 2.0f;

	SabrinaEspadaMovDelante = *app->animationManager->GetAnimByName("SabrinaEspadaCaminar_delante");
	SabrinaEspadaMovDelante.speed = 2.0f;

	SabrinaCetroMovDelante = *app->animationManager->GetAnimByName("SabrinaCetroCaminar_delante");
	SabrinaCetroMovDelante.speed = 2.0f;

	SabrinaEspadaMovDerecha = *app->animationManager->GetAnimByName("SabrinaEspadaCaminar_derecha");
	SabrinaEspadaMovDerecha.speed = 2.0f;

	SabrinaCetroMovDerecha = *app->animationManager->GetAnimByName("SabrinaCetroCaminar_derecha");
	SabrinaCetroMovDerecha.speed = 2.0f;

	SabrinaEspadaMovDetras = *app->animationManager->GetAnimByName("SabrinaEspadaCaminar_detras");
	SabrinaEspadaMovDetras.speed = 2.0f;

	SabrinaCetroMovDetras = *app->animationManager->GetAnimByName("SabrinaCetroCaminar_detras");
	SabrinaCetroMovDetras.speed = 2.0f;

	SabrinaEspadaDano = *app->animationManager->GetAnimByName("SabrinaEspadaDano");
	SabrinaEspadaDano.speed = 2.0f;

	SabrinaCetroDano = *app->animationManager->GetAnimByName("SabrinaCetroDano");
	SabrinaCetroDano.speed = 2.0f;

	SabrinaEspadaDash = *app->animationManager->GetAnimByName("SabrinaEspadaDash");
	SabrinaEspadaDash.speed = 2.0f;

	SabrinaCetroDash = *app->animationManager->GetAnimByName("SabrinaCetroDash");
	SabrinaCetroDash.speed = 2.0f;

	SabrinaEspadaMuerte = *app->animationManager->GetAnimByName("SabrinaEspadaMuerte");
	SabrinaEspadaMuerte.speed = 2.0f;

	SabrinaCetroMuerte = *app->animationManager->GetAnimByName("SabrinaCetroMuerte");
	SabrinaCetroMuerte.speed = 2.0f;

	SabrinaEspadaRecolectar = *app->animationManager->GetAnimByName("SabrinaEspadaRecolectar");
	SabrinaEspadaRecolectar.speed = 2.0f;

	SabrinaCetroRecolectar = *app->animationManager->GetAnimByName("SabrinaCetroRecolectar");
	SabrinaCetroRecolectar.speed = 2.0f;

	SabrinaAtaque = *app->animationManager->GetAnimByName("SabrinaAtaque");
	SabrinaAtaque.speed = 2.0f;

	totalLivesPlayer = livesPlayer;

	sceneChange = false;

	return true;
}

bool Player::Update(float dt)
{
	movementFSM->Update(dt);
	combatFSM->Update(dt);

	pbody->body->SetTransform(pbody->body->GetPosition(), 0);

	if (vida <= 0.0f) {
		pbody->body->SetTransform({ PIXEL_TO_METERS(672),PIXEL_TO_METERS(2032) }, 0);
		vida = 10.0f;
	}
	
	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 46;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 64;

	app->render->DrawTexture(texture, position.x - 15, position.y - 25);

	b2Vec2 mouseWorldPosition = { PIXEL_TO_METERS(app->input->GetMouseX()) + PIXEL_TO_METERS(-app->render->camera.x), PIXEL_TO_METERS(app->input->GetMouseY()) + PIXEL_TO_METERS(-app->render->camera.y) };

	lookingDir = mouseWorldPosition - pbody->body->GetPosition();
	lookingDir.Normalize();

	lookingAngle = -app->physics->lookAt(b2Vec2(1, 0), lookingDir) * 2;

	if (debug) {
		mouseWorldPosition = { PIXEL_TO_METERS(app->input->GetMouseX()) + PIXEL_TO_METERS(-app->render->camera.x), PIXEL_TO_METERS(app->input->GetMouseY()) + PIXEL_TO_METERS(-app->render->camera.y) };
		app->render->DrawLine(METERS_TO_PIXELS(pbody->body->GetPosition().x), METERS_TO_PIXELS(pbody->body->GetPosition().y), METERS_TO_PIXELS(mouseWorldPosition.x), METERS_TO_PIXELS(mouseWorldPosition.y), 255, 0, 0);
		app->render->DrawLine(METERS_TO_PIXELS(pbody->body->GetPosition().x), METERS_TO_PIXELS(pbody->body->GetPosition().y), METERS_TO_PIXELS(pbody->body->GetPosition().x) + pbody->body->GetLinearVelocity().x*10, METERS_TO_PIXELS(pbody->body->GetPosition().y) + + pbody->body->GetLinearVelocity().y * 10, 255, 255, 0);
		if (app->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN) {
			freeCam = !freeCam;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
	{
		if (currentClass == PlayerClass::KNIGHT)
		{
			currentClass = PlayerClass::WIZARD;
		}
		else
		{
			currentClass = PlayerClass::KNIGHT;
		}
	}

	if (livesPlayer == 0) deadPlayer;


	//----Scene Change Management----
	if(sceneChange)
	{
		if (app->sceneManager->GetCurrentScene()->name == "tutorialscene")
		{
			app->sceneManager->ChangeScene("townscene");
		}
		if (app->sceneManager->GetCurrentScene()->name == "townscene")
		{
			app->sceneManager->ChangeScene("forestscene");
		}
		if (app->sceneManager->GetCurrentScene()->name == "forestscene")
		{
			app->sceneManager->ChangeScene("townscene");
		}
	}



	return true;
}

void Player::DrawImGui()
{
	ImGui::Begin("Player");

	ImGui::Text("Player Position: %d, %d", position.x, position.y);
	ImGui::Text("Player Lives: %d", livesPlayer);
	ImGui::Text("Player Mana: %d", mana);
	ImGui::Text("Player Speed: %f", pbody->body->GetLinearVelocity().Length());
	ImGui::SliderFloat("max speed", &maxSpeed, 1.0f, 10.0f);
	ImGui::SliderFloat("move force", &moveForce, 1.0f, 10.0f);

	ImGui::Text("Looking Dir: (%f, %f)", lookingDir.x, lookingDir.y);
	ImGui::Text("Looking Angle: %f", lookingAngle);

	inventory.DrawImGui();

	ImGui::Text("Player Class: %s", currentClass == PlayerClass::KNIGHT ? "KNIGHT" : "WIZARD");

	ImGui::Text("player hurt cooldown: %f", playerHurtCultdown.ReadMSec());

	ImGui::End();
}

bool Player::SaveState(pugi::xml_node& node) {

	pugi::xml_node playerAttributes = node.append_child("player");
	playerAttributes.append_attribute("x").set_value(this->position.x);
	playerAttributes.append_attribute("y").set_value(this->position.y);

	return true;
}

bool Player::LoadState(pugi::xml_node& node)
{
	pbody->body->SetTransform({ PIXEL_TO_METERS(node.child("player").attribute("x").as_int()), PIXEL_TO_METERS(node.child("player").attribute("y").as_int()) }, node.child("player").attribute("angle").as_int());
	// reset player physics
	pbody->body->SetAwake(false);
	pbody->body->SetAwake(true);

	return true;
}

bool Player::CleanUp() {

	app->tex->UnLoad(texture);
	app->physics->DestroyBody(pbody);

	return true;
}

//TODO arreglar esta cochinada
void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::CHANGESCENE:
		sceneChange = true;
		break;
		//TODO: He a�adido esto para probar que la pocion de curar funcione, se puede borrar :)
	case ColliderType::ENEMY:
		if(playerHurtCultdown.ReadMSec() > 1000.0f)
			{
				livesPlayer--; 
				playerHurtCultdown.Start();
			}
		break;
	}

	
}

void Player::EndCollision(PhysBody* physA, PhysBody* physB){
	
}

void Player::OnRaycastHit(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction){
	
}
