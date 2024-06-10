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

	totalLivesPlayer = livesPlayer;

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

	return true;
}

bool Player::Update(float dt)
{
	//CHEATS
	if (godMode) {
		vida = 10.0f;
		livesPlayer = 10;
	}
	if (ghostMode) {
		pbody->body->GetFixtureList()->SetSensor(ghostMode);
		SDL_SetTextureAlphaMod(texture, 100);
	}
	else {
		SDL_SetTextureAlphaMod(texture, 255);
	}

	movementFSM->Update(dt);
	combatFSM->Update(dt);

	inventory.Update(dt);

	pbody->body->SetTransform(pbody->body->GetPosition(), 0);

	if (vida <= 0.0f) {
		pbody->body->SetTransform({ PIXEL_TO_METERS(672),PIXEL_TO_METERS(2032) }, 0); //TODO: QUITAR ESTO!!! TIENE QUE SER EL SPAWNPOINT DEL PLAYER EN ESE MAPA
		vida = 10.0f;
	}
	
	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 46;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 64;

	damage->position = { position.x + 46, position.y + 64};

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

	ImGui::Text("Player Movement State: %s", movementFSM->GetCurrentState().name.GetString());
	ImGui::Text("Player Combat State: %s", combatFSM->GetCurrentState().name.GetString());

	ImGui::Text("player hurt cooldown: %f", playerHurtCultdown.ReadMSec());

	ImGui::Text("dash timer: %f", dashTimer.ReadMSec());

	ImGui::Text("Player level: %d", level);
	
	if (ImGui::Button("Add Level"))
		level++;
	ImGui::SameLine();
	if (ImGui::Button("Remove Level")) {
		if (level > 0)
			level--;
	}

	ImGui::Text("Player abylity points: %d", abylityPoints);
	
	if (ImGui::Button("Add ability Point"))
		abylityPoints++;
	ImGui::SameLine();
	if (ImGui::Button("Remove ability Point")) {
		if (abylityPoints > 0)
			abylityPoints--;
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
	playerAttributes.append_attribute("lives").set_value(this->livesPlayer);
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
	bestiaryNode.append_attribute("mission1Completed").set_value(bestiary->mission1Completed);
	bestiaryNode.append_attribute("mission2Completed").set_value(bestiary->mission2Completed);

	bestiaryNode.append_attribute("swordAbility100Unlocked").set_value(bestiary->swordAbility100Unlocked);
	bestiaryNode.append_attribute("swordAbility110Unlocked").set_value(bestiary->swordAbility110Unlocked);
	bestiaryNode.append_attribute("swordAbility111Unlocked").set_value(bestiary->swordAbility111Unlocked);
	bestiaryNode.append_attribute("swordAbility112Unlocked").set_value(bestiary->swordAbility112Unlocked);
	bestiaryNode.append_attribute("swordAbility120Unlocked").set_value(bestiary->swordAbility120Unlocked);
	bestiaryNode.append_attribute("swordAbility122Unlocked").set_value(bestiary->swordAbility122Unlocked);
	bestiaryNode.append_attribute("swordAbility123Unlocked").set_value(bestiary->swordAbility123Unlocked); 

	bestiaryNode.append_attribute("staffAbility100Unlocked").set_value(bestiary->staffAbility100Unlocked);
	bestiaryNode.append_attribute("staffAbility110Unlocked").set_value(bestiary->staffAbility110Unlocked);
	bestiaryNode.append_attribute("staffAbility111Unlocked").set_value(bestiary->staffAbility111Unlocked); 
	bestiaryNode.append_attribute("staffAbility112Unlocked").set_value(bestiary->staffAbility112Unlocked);
	bestiaryNode.append_attribute("staffAbility120Unlocked").set_value(bestiary->staffAbility120Unlocked);
	bestiaryNode.append_attribute("staffAbility122Unlocked").set_value(bestiary->staffAbility122Unlocked);
	bestiaryNode.append_attribute("staffAbility123Unlocked").set_value(bestiary->staffAbility123Unlocked);
	
	// Pag 2 attributes
	bestiaryNode.append_attribute("enemy1Killed").set_value(bestiary->enemy1Killed);
	bestiaryNode.append_attribute("enemy2Killed").set_value(bestiary->enemy2Killed);
	bestiaryNode.append_attribute("enemy3Killed").set_value(bestiary->enemy3Killed);

	// Pag 3 attributes
	bestiaryNode.append_attribute("hepaticaPlantCollected").set_value(bestiary->hepaticaPlantCollected); 
	bestiaryNode.append_attribute("comfreyPlantCollected").set_value(bestiary->comfreyPlantCollected);
	bestiaryNode.append_attribute("hawthornPlantCollected").set_value(bestiary->hawthornPlantCollected);
	bestiaryNode.append_attribute("witchhazelPlantCollected").set_value(bestiary->witchhazelPlantCollected);
	bestiaryNode.append_attribute("arnicaPlantCollected").set_value(bestiary->arnicaPlantCollected);

	// Pag 4 attributes
	bestiaryNode.append_attribute("klausUnlocked").set_value(bestiary->klausUnlocked); 
	bestiaryNode.append_attribute("bountyUnlocked").set_value(bestiary->bountyUnlocked);

	//QUESTS SAVE

	return true;
}

bool Player::LoadState(pugi::xml_node& node)
{
	pugi::xml_node playerNode = node.child("player");

	pbody->body->SetTransform({ PIXEL_TO_METERS(node.child("player").attribute("x").as_int()), PIXEL_TO_METERS(node.child("player").attribute("y").as_int()) }, node.child("player").attribute("angle").as_int());
	// reset player physics
	pbody->body->SetAwake(false);
	pbody->body->SetAwake(true);

	this->livesPlayer = node.child("player").attribute("lives").as_int();
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
	bestiary->mission1Completed = bestiaryNode.attribute("mission1Completed").as_bool();
	bestiary->mission2Completed = bestiaryNode.attribute("mission2Completed").as_bool();

	bestiary->swordAbility100Unlocked = bestiaryNode.attribute("swordAbility100Unlocked").as_bool();
	bestiary->swordAbility110Unlocked = bestiaryNode.attribute("swordAbility110Unlocked").as_bool();
	bestiary->swordAbility111Unlocked = bestiaryNode.attribute("swordAbility111Unlocked").as_bool();
	bestiary->swordAbility112Unlocked = bestiaryNode.attribute("swordAbility112Unlocked").as_bool();
	bestiary->swordAbility120Unlocked = bestiaryNode.attribute("swordAbility120Unlocked").as_bool();
	bestiary->swordAbility122Unlocked = bestiaryNode.attribute("swordAbility122Unlocked").as_bool();
	bestiary->swordAbility123Unlocked = bestiaryNode.attribute("swordAbility123Unlocked").as_bool();

	bestiary->staffAbility100Unlocked = bestiaryNode.attribute("staffAbility100Unlocked").as_bool();
	bestiary->staffAbility110Unlocked = bestiaryNode.attribute("staffAbility110Unlocked").as_bool();
	bestiary->staffAbility111Unlocked = bestiaryNode.attribute("staffAbility111Unlocked").as_bool();
	bestiary->staffAbility112Unlocked = bestiaryNode.attribute("staffAbility112Unlocked").as_bool();
	bestiary->staffAbility120Unlocked = bestiaryNode.attribute("staffAbility120Unlocked").as_bool();
	bestiary->staffAbility122Unlocked = bestiaryNode.attribute("staffAbility122Unlocked").as_bool();
	bestiary->staffAbility123Unlocked = bestiaryNode.attribute("staffAbility123Unlocked").as_bool();

	// Pag 2 attributes
	bestiary->enemy1Killed = bestiaryNode.attribute("enemy1Killed").as_bool();
	bestiary->enemy2Killed = bestiaryNode.attribute("enemy2Killed").as_bool(); 
	bestiary->enemy3Killed = bestiaryNode.attribute("enemy3Killed").as_bool(); 

	// Pag 3 attributes
	bestiary->hepaticaPlantCollected = bestiaryNode.attribute("hepaticaPlantCollected").as_bool();
	bestiary->comfreyPlantCollected = bestiaryNode.attribute("comfreyPlantCollected").as_bool();
	bestiary->hawthornPlantCollected = bestiaryNode.attribute("hawthornPlantCollected").as_bool();
	bestiary->witchhazelPlantCollected = bestiaryNode.attribute("witchhazelPlantCollected").as_bool();
	bestiary->arnicaPlantCollected = bestiaryNode.attribute("arnicaPlantCollected").as_bool();

	// Pag 4 attributes
	bestiary->klausUnlocked = bestiaryNode.attribute("klausUnlocked").as_bool();
	bestiary->bountyUnlocked = bestiaryNode.attribute("bountyUnlocked").as_bool();

	//QUESTS LOAD

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
				damage->emiting = true;
				playerHurtCultdown.Start();
			}
		break;
	case ColliderType::BULLET:
		vida -= 2.0f;
		damage->emiting = true;
		break;
	}

	
}

void Player::EndCollision(PhysBody* physA, PhysBody* physB){
	
}

void Player::OnRaycastHit(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction){
	
}
