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
#include "Core/ParticleManager.h"
#include "Core/Lighting.h"
#include "Core/AnimationManager.h"
#include "Core/Audio.h"

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

	bestiary = new Bestiary();

	dashTimer = Timer();

	texture = app->tex->Load("Assets/Textures/playerx128-test.png");
	texture1 = app->tex->Load("Assets/Textures/sombraSabrina.png");

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

	// Audios

	stepsFx = app->audio->LoadFx(parameters.attribute("stepsFx").as_string());
	swordSlashFx = app->audio->LoadFx(parameters.attribute("swordSlashFx").as_string());
	firestaffFx = app->audio->LoadFx(parameters.attribute("fireStaffFx").as_string());
	damagedSabrinaFx = app->audio->LoadFx(parameters.attribute("damagedSabrinaFx").as_string());
	blockFx = app->audio->LoadFx(parameters.attribute("blockFx").as_string());
	potionFx = app->audio->LoadFx(parameters.attribute("potionFx").as_string());

	//Anims
	SabrinaEspadaIdle = *app->animationManager->GetAnimByName("SabrinaEspadaIdle_1");
	SabrinaEspadaIdle.speed = 0.5f;

	SabrinaCetroIdle = *app->animationManager->GetAnimByName("SabrinaCetroIdle");
	SabrinaCetroIdle.speed = 1.0f;

	SabrinaEspadaMovDelante = *app->animationManager->GetAnimByName("SabrinaEspadaCaminar_delante");
	SabrinaEspadaMovDelante.speed = 2.0f;

	SabrinaCetroMovDelante = *app->animationManager->GetAnimByName("SabrinaCetroCaminar_delante");
	SabrinaCetroMovDelante.speed = 2.0f;

	SabrinaEspadaMovDerecha = *app->animationManager->GetAnimByName("SabrinaEspadaCaminar_derecha");
	SabrinaEspadaMovDerecha.speed = 2.0f;

	SabrinaCetroMovDerecha = *app->animationManager->GetAnimByName("SabrinaCetroCaminar_derecha");
	SabrinaCetroMovDerecha.speed = 2.0f;

	SabrinaEspadaMovIzquierda = *app->animationManager->GetAnimByName("SabrinaEspadaCaminar_izquierda");
	SabrinaEspadaMovIzquierda.speed = 2.0f;

	SabrinaCetroMovIzquierda = *app->animationManager->GetAnimByName("SabrinaCetroCaminar_izquierda");
	SabrinaCetroMovIzquierda.speed = 2.0f;

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

	SabrinaEspadaAtaque = *app->animationManager->GetAnimByName("SabrinaEspadaAtaque");
	SabrinaEspadaAtaque.speed = 2.0f;

	SabrinaCetroAtaque = *app->animationManager->GetAnimByName("SabrinaCetroAtaque");
	SabrinaCetroAtaque.speed = 2.0f;

	sceneChange = false;

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
	damage->color = { 255, 0, 0, 128 };
	app->particleManager->AddGenerator(damage);

/* 	playerLight = app->lighting->AddLight({ position.x, position.y });
	playerLight->radius = 5;
	playerLight->color = { 255, 191, 87, 255 }; */


	return true;
}

bool Player::Update(float dt)
{
	/* playerLight->position = { position.x, position.y }; */
	timerSteps += dt;
	timerSword += dt;

	//CHEATS
	if (godMode) {
		vida = 15.0f;
		mana = 100.0f;
	}
	if (ghostMode) {
		pbody->body->GetFixtureList()->SetSensor(ghostMode);
		SDL_SetTextureAlphaMod(texture, 100);
	}
	else {
		SDL_SetTextureAlphaMod(texture, 255);
	}

	if (currentClass == PlayerClass::KNIGHT)
	{
		defense = 15.0f;
	}
	else
	{
		defense = 3.0f;
	}

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 46;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 64;

	shadowPosition = {position.x - 20, position.y + 150};

	movementFSM->Update(dt);
	combatFSM->Update(dt);

	pbody->body->SetTransform(pbody->body->GetPosition(), 0);

	//if (vida <= 0.0f) {
	//	pbody->body->SetTransform({ PIXEL_TO_METERS(672),PIXEL_TO_METERS(2032) }, 0); //TODO: QUITAR ESTO!!! TIENE QUE SER EL SPAWNPOINT DEL PLAYER EN ESE MAPA
	//	vida = maxVida;
	//}

	//app->render->DrawTexture(texture, position.x - 15, position.y - 25);
	damage->position = { position.x + 46, position.y + 64};

	//Animations
	SDL_Rect currentFrame = currentAnim->GetCurrentFrame();
	int textureX = position.x - (currentFrame.w / 2) - 15;
	int textureY = position.y - (currentFrame.h / 2) - 25;
	//Renderizar la animación actual
	if (currentAnim != nullptr) {
		app->render->DrawTexture(currentAnim->texture, position.x - 40, position.y - 80, &currentAnim->GetCurrentFrame(), 1.0f, 0.0f, 1.0f, 3, flip);
	}
	else {
		app->render->DrawTexture(texture, position.x - 40, position.y - 80);
	}

	app->render->DrawTexture(texture1, shadowPosition.x, shadowPosition.y);

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

	if(app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
	{
		if(currentPotion != nullptr)
		{
			currentPotion->Use();
		}
		else
		{
			LOG("No potion selected!!");
		}
	}

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

	//NO borrar T-T
	if (vida <= 0) {
		vida = 0.0f;
		deadPlayer = true;
	}

	experienceToNextLevel = level * 50;
	if (currentExperience >= experienceToNextLevel and level <= levelCap)
	{
		currentExperience -= experienceToNextLevel;
		level++;
		abilityPoints++;
	}

	return true;
}

void Player::DrawImGui()
{
	ImGui::Begin("Player");

	ImGui::Text("Player Position: %d, %d", position.x, position.y);
	ImGui::Text("Player Vida: %f", vida);
	ImGui::Text("Player Mana: %f", mana);
	ImGui::Text("Player Mana Regeneration: %f", manaRegeneration);
	ImGui::Text("Player Speed: %f", pbody->body->GetLinearVelocity().Length());
	ImGui::SliderFloat("max speed", &maxSpeed, 1.0f, 10.0f);
	ImGui::SliderFloat("move force", &moveForce, 1.0f, 10.0f);

	ImGui::Text("Looking Dir: (%f, %f)", lookingDir.x, lookingDir.y);
	ImGui::Text("Looking Angle: %f", lookingAngle);

	inventory.DrawImGui();

	ImGui::Text("Player Class: %s", currentClass == PlayerClass::KNIGHT ? "KNIGHT" : "WIZARD");

	ImGui::Text("player hurt cooldown: %f", playerHurtCultdown.ReadMSec());

	ImGui::Text("dash timer: %f", dashTimer.ReadMSec());

	ImGui::Text("Player level: %d", level);
	ImGui::Text("Player experience: %d", currentExperience);

	ImGui::Text("Player Strength: %f", strength);
	ImGui::Text("Player Intelligence: %f", intelligence);

	ImGui::Text("Player Paralysis: %s", paralysis ? "true" : "false");

	ImGui::Text("Blood: %s", bleed ? "true" : "false");
	ImGui::Text("Bleed chance: %d", bleedChance);
	
	if (ImGui::Button("Add Level"))
		level++;
	ImGui::SameLine();
	if (ImGui::Button("Remove Level")) {
		if (level > 0)
			level--;
	}

	ImGui::Text("Player abylity points: %d", abilityPoints);
	
	if (ImGui::Button("Add ability Point"))
		abilityPoints++;
	ImGui::SameLine();
	if (ImGui::Button("Remove ability Point")) {
		if (abilityPoints > 0)
			abilityPoints--;
	}
	ImGui::Separator();
	ImGui::Text("Player Cheats");
	ImGui::Checkbox("God Mode", &godMode);
	ImGui::Checkbox("Ghost Mode", &ghostMode);

	ImGui::End();
}

bool Player::SaveState(pugi::xml_node& node) {

	pugi::xml_node playerAttributes = node.append_child("player");
	playerAttributes.append_attribute("x").set_value(this->position.x);
	playerAttributes.append_attribute("y").set_value(this->position.y);
	playerAttributes.append_attribute("lives").set_value(this->vida);
	playerAttributes.append_attribute("mana").set_value(this->mana);
	playerAttributes.append_attribute("Level").set_value(this->level);

	//INVENTORY SAVE
	Inventory* playerInventory = &app->entityManager->GetPlayerEntity()->inventory;
	for (int i = 0; i < playerInventory->items.size(); i++)
	{
		Item* item = playerInventory->items[i];
		pugi::xml_node itemNode = playerAttributes.append_child("inventory"); 
		itemNode.append_attribute("name").set_value(item->name.c_str());
		itemNode.append_attribute("quantity").set_value(item->quantity);
	}

	//BESTIARY SAVE
	pugi::xml_node bestiaryNode = playerAttributes.append_child("bestiary");
	// Pag 1 attributes
	bestiaryNode.append_child("pag1").append_attribute("mission1Completed").set_value(bestiary->mission1Completed);
	bestiaryNode.append_child("pag1").append_attribute("mission2Completed").set_value(bestiary->mission2Completed);

	bestiaryNode.append_child("pag1").append_attribute("swordAbility100Unlocked").set_value(bestiary->swordAbility100Unlocked);
	bestiaryNode.append_child("pag1").append_attribute("swordAbility110Unlocked").set_value(bestiary->swordAbility110Unlocked);
	bestiaryNode.append_child("pag1").append_attribute("swordAbility111Unlocked").set_value(bestiary->swordAbility111Unlocked);
	bestiaryNode.append_child("pag1").append_attribute("swordAbility112Unlocked").set_value(bestiary->swordAbility112Unlocked);
	bestiaryNode.append_child("pag1").append_attribute("swordAbility120Unlocked").set_value(bestiary->swordAbility120Unlocked);
	bestiaryNode.append_child("pag1").append_attribute("swordAbility122Unlocked").set_value(bestiary->swordAbility122Unlocked);
	bestiaryNode.append_child("pag1").append_attribute("swordAbility123Unlocked").set_value(bestiary->swordAbility123Unlocked);

	bestiaryNode.append_child("pag1").append_attribute("staffAbility100Unlocked").set_value(bestiary->staffAbility100Unlocked);
	bestiaryNode.append_child("pag1").append_attribute("staffAbility110Unlocked").set_value(bestiary->staffAbility110Unlocked);
	bestiaryNode.append_child("pag1").append_attribute("staffAbility111Unlocked").set_value(bestiary->staffAbility111Unlocked);
	bestiaryNode.append_child("pag1").append_attribute("staffAbility112Unlocked").set_value(bestiary->staffAbility112Unlocked);
	bestiaryNode.append_child("pag1").append_attribute("staffAbility120Unlocked").set_value(bestiary->staffAbility120Unlocked);
	bestiaryNode.append_child("pag1").append_attribute("staffAbility122Unlocked").set_value(bestiary->staffAbility122Unlocked);
	bestiaryNode.append_child("pag1").append_attribute("staffAbility123Unlocked").set_value(bestiary->staffAbility123Unlocked);
	
	// Pag 2 attributes
	bestiaryNode.append_child("pag2").append_attribute("enemy1Killed").set_value(bestiary->enemy1Killed);
	bestiaryNode.append_child("pag2").append_attribute("enemy2Killed").set_value(bestiary->enemy2Killed);
	bestiaryNode.append_child("pag2").append_attribute("enemy3Killed").set_value(bestiary->enemy3Killed);

	// Pag 3 attributes
	bestiaryNode.append_child("pag3").append_attribute("hepaticaPlantCollected").set_value(bestiary->hepaticaPlantCollected);
	bestiaryNode.append_child("pag3").append_attribute("comfreyPlantCollected").set_value(bestiary->comfreyPlantCollected);
	bestiaryNode.append_child("pag3").append_attribute("hawthornPlantCollected").set_value(bestiary->hawthornPlantCollected);
	bestiaryNode.append_child("pag3").append_attribute("witchhazelPlantCollected").set_value(bestiary->witchhazelPlantCollected);
	bestiaryNode.append_child("pag3").append_attribute("arnicaPlantCollected").set_value(bestiary->arnicaPlantCollected);

	// Pag 4 attributes
	bestiaryNode.append_child("pag4").append_attribute("klausUnlocked").set_value(bestiary->klausUnlocked);
	bestiaryNode.append_child("pag4").append_attribute("bountyUnlocked").set_value(bestiary->bountyUnlocked);

	return true;
}

bool Player::LoadState(pugi::xml_node& node)
{
	pugi::xml_node playerNode = node.child("player");

	pbody->body->SetTransform({ PIXEL_TO_METERS(node.child("player").attribute("x").as_int()), PIXEL_TO_METERS(node.child("player").attribute("y").as_int()) }, node.child("player").attribute("angle").as_int());
	// reset player physics
	pbody->body->SetAwake(false);
	pbody->body->SetAwake(true);

	this->vida = node.child("player").attribute("lives").as_int();
	this->mana = node.child("player").attribute("mana").as_int();
	this->level = node.child("player").attribute("Level").as_int();

	//INVENTORY LOAD
	Inventory* playerInventory = &app->entityManager->GetPlayerEntity()->inventory;
	for (pugi::xml_node itemNode = playerNode.child("inventory"); itemNode; itemNode = itemNode.next_sibling("inventory")) {
		std::string itemName = itemNode.attribute("name").as_string();
		int itemQuantity = itemNode.attribute("quantity").as_int();

		app->entityManager->GetPlayerEntity()->inventory.AddNItems(itemName, itemQuantity);
	}

	//BESTIARY LOAD
	pugi::xml_node bestiaryNode = playerNode.child("bestiary");
	// Pag 1 attributes
	bestiary->mission1Completed = bestiaryNode.child("pag1").attribute("mission1Completed").as_bool();
	bestiary->mission2Completed = bestiaryNode.child("pag1").attribute("mission2Completed").as_bool();

	bestiary->swordAbility100Unlocked = bestiaryNode.child("pag1").attribute("swordAbility100Unlocked").as_bool();
	bestiary->swordAbility110Unlocked = bestiaryNode.child("pag1").attribute("swordAbility110Unlocked").as_bool();
	bestiary->swordAbility111Unlocked = bestiaryNode.child("pag1").attribute("swordAbility111Unlocked").as_bool();
	bestiary->swordAbility112Unlocked = bestiaryNode.child("pag1").attribute("swordAbility112Unlocked").as_bool();
	bestiary->swordAbility120Unlocked = bestiaryNode.child("pag1").attribute("swordAbility120Unlocked").as_bool();
	bestiary->swordAbility122Unlocked = bestiaryNode.child("pag1").attribute("swordAbility122Unlocked").as_bool();
	bestiary->swordAbility123Unlocked = bestiaryNode.child("pag1").attribute("swordAbility123Unlocked").as_bool();

	bestiary->staffAbility100Unlocked = bestiaryNode.child("pag1").attribute("staffAbility100Unlocked").as_bool();
	bestiary->staffAbility110Unlocked = bestiaryNode.child("pag1").attribute("staffAbility110Unlocked").as_bool();
	bestiary->staffAbility111Unlocked = bestiaryNode.child("pag1").attribute("staffAbility111Unlocked").as_bool();
	bestiary->staffAbility112Unlocked = bestiaryNode.child("pag1").attribute("staffAbility112Unlocked").as_bool();
	bestiary->staffAbility120Unlocked = bestiaryNode.child("pag1").attribute("staffAbility120Unlocked").as_bool();
	bestiary->staffAbility122Unlocked = bestiaryNode.child("pag1").attribute("staffAbility122Unlocked").as_bool();
	bestiary->staffAbility123Unlocked = bestiaryNode.child("pag1").attribute("staffAbility123Unlocked").as_bool();

	// Pag 2 attributes
	bestiary->enemy1Killed = bestiaryNode.child("pag2").attribute("enemy1Killed").as_bool();
	bestiary->enemy2Killed = bestiaryNode.child("pag2").attribute("enemy2Killed").as_bool();
	bestiary->enemy3Killed = bestiaryNode.child("pag2").attribute("enemy3Killed").as_bool();

	// Pag 3 attributes
	bestiary->hepaticaPlantCollected = bestiaryNode.child("pag3").attribute("hepaticaPlantCollected").as_bool();
	bestiary->comfreyPlantCollected = bestiaryNode.child("pag3").attribute("comfreyPlantCollected").as_bool();
	bestiary->hawthornPlantCollected = bestiaryNode.child("pag3").attribute("hawthornPlantCollected").as_bool();
	bestiary->witchhazelPlantCollected = bestiaryNode.child("pag3").attribute("witchhazelPlantCollected").as_bool();
	bestiary->arnicaPlantCollected = bestiaryNode.child("pag3").attribute("arnicaPlantCollected").as_bool();

	// Pag 4 attributes
	bestiary->klausUnlocked = bestiaryNode.child("pag4").attribute("klausUnlocked").as_bool();
	bestiary->bountyUnlocked = bestiaryNode.child("pag4").attribute("bountyUnlocked").as_bool();

	return true; 
}

#pragma region AbilitiesUnlock

void Player::AbilitySword100() { // DONE
	// Increase player strength +3
	strength += 13.0f;
}

void Player::AbilitySword110() { // DONE
	// 15% chance of bleed effect
	bleed = true;
}

void Player::AbilitySword111() {
	// When parry, stack armor 3 times
}

void Player::AbilitySword112() {
	// Double attack
}

void Player::AbilitySword120() {
	// +20% life regeneration
	stealLife = true;
}

void Player::AbilitySword122() {
	// Double attack
}

void Player::AbilitySword123() {
	// Area sword attack 100% chance of bleed
	// bleedChance = 100;
}

void Player::AbilityStaff100() { // DONE
	// Increase player intelligence +3
	intelligence = 17.0f;
}

void Player::AbilityStaff110() {
	// +10% chance of paralysis effect
	paralysis = true;
}

void Player::AbilityStaff111() {
	// Tornado area that slows enemies
}

void Player::AbilityStaff112() {
	// Double projectile
}

void Player::AbilityStaff120() {
	// +15% energy regeneration
	manaRegeneration = 5.0f;
}

void Player::AbilityStaff122() {
	// Double projectile
}

void Player::AbilityStaff123() {
	// Fireball that can pass through enemies and have a 70% chance of burn
}

#pragma endregion AbilitiesUnlock

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
	case ColliderType::ENEMYWASP:
		if(playerHurtCultdown.ReadMSec() > 1000.0f)
		{
			vida -= 5.0f;
			app->audio->PlayFx(damagedSabrinaFx);
			damage->emiting = true;
			playerHurtCultdown.Start();
		}
		break;
	case ColliderType::ENEMYSPIDER:
		if (playerHurtCultdown.ReadMSec() > 1000.0f)
		{
			vida -= 5.0f;
			app->audio->PlayFx(damagedSabrinaFx);
			damage->emiting = true;
			playerHurtCultdown.Start();
		}
		break;
	case ColliderType::ENEMYBOSS:
		if (playerHurtCultdown.ReadMSec() > 1000.0f)
		{
			vida -= 13.0f;
			app->audio->PlayFx(damagedSabrinaFx);
			damage->emiting = true;
			playerHurtCultdown.Start();
		}
		break;
	case ColliderType::BULLET:
		vida -= 7.0f;
		app->audio->PlayFx(damagedSabrinaFx);
		damage->emiting = true;
		break;
	}

	
}

void Player::EndCollision(PhysBody* physA, PhysBody* physB){
	
}

void Player::OnRaycastHit(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction){
	
}
