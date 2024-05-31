#include "Gameplay/Entities/Items/Potion.h"

#include "Core/Audio.h"

#include "Utils/Log.h"

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
#include "Core/Window.h"
#include "Core/SceneManager.h"
#include "Core/Map.h"

#include <cstdlib>

#ifdef __linux__
#include <Box2D/Dynamics/b2Body.h>
#endif

Potion::Potion() : Entity(EntityType::POTION)
{
	name.Create("potion");
}

Potion::~Potion() {}

bool Potion::Awake() {

	return true;
}

bool Potion::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	type = parameters.attribute("type").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	textureSelection = parameters.attribute("textureSelection").as_string();

	texture = app->tex->Load(texturePath);
	texture2 = app->tex->Load(textureSelection);

	pbody = app->physics->CreateRectangle(position.x, position.y, 80, 80, bodyType::STATIC);
	pbody->ctype = ColliderType::POTION;
	pbody->listener = this;

	return true;
}

bool Potion::Update(float dt)
{
	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	app->input->GetMousePosition(mouseX, mouseY); 
	mouseX *= app->win->GetScale();
	mouseY *= app->win->GetScale();

	Player* player;
	player = app->entityManager->GetPlayerEntity();

	// L07 DONE 4: Add a physics to an food - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 8;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 8;

	posX = (int)windowW / 2 - 350 + position.x;
	posY = (int)windowH / 2 - 350 + position.y;

	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT) {
		//TODO: Cada pocion hace su cosa
		if (isCreated) {
			LOG("type: %s, mouseX: %d, mouseY: %d, posX: %d, posY : %d", this->name,mouseX, mouseY, posX, posY);
			//TODO: Dibujar la textura de la pocion
			if (mouseX >= posX + 50 && mouseX <= posX + 350 && mouseY >= posY + 50 && mouseY <= posY + 350) {
				//selected = true;
				app->render->DrawTexture(texture2, posX, posY, 0, 0);
			}
			else {
				app->render->DrawTexture(texture, posX, posY, 0, 0);
			}
		}
	}

	return true;        
}

bool Potion::CleanUp()
{
	app->physics->DestroyBody(pbody);
	return true;
}

void Potion::OnCollision(PhysBody* physA, PhysBody* physB) {

}

void Potion::EndCollision(PhysBody* physA, PhysBody* physB)
{
	
}

bool Potion::SaveState(pugi::xml_node& node) {
	Player* player; 
	player = app->entityManager->GetPlayerEntity(); 

	pugi::xml_node potionAttributes = node.append_child("potion");
	potionAttributes.append_attribute("healCounter").set_value(player->healPlantCounter);
	potionAttributes.append_attribute("veloCounter").set_value(player->veloPlantCounter);
	potionAttributes.append_attribute("energyCounter").set_value(player->energyPlantCounter);

	return true;
}

bool Potion::LoadState(pugi::xml_node& node)
{
	Player* player;
	player = app->entityManager->GetPlayerEntity();

	player->healPlantCounter = node.child("potion").attribute("healCounter").as_int(); 
	player->veloPlantCounter = node.child("potion").attribute("veloCounter").as_int();
	player->energyPlantCounter = node.child("potion").attribute("energyCounter").as_int();

	return true;
}
