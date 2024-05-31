#include "Gameplay/Entities/Items/Plant.h"

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
#include "Core/SceneManager.h"
#include "Core/Map.h"

#include <cstdlib>

Plant::Plant() : Entity(EntityType::PLANT)
{
	name.Create("plant");
}

Plant::~Plant() {}

bool Plant::Awake() {

	return true;
}

bool Plant::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	type = parameters.attribute("type").as_int(); 

	texture = app->tex->Load(texturePath);
	texture1 = app->tex->Load("Assets/Textures/pressE.png");

	pbody = app->physics->CreateRectangle(position.x, position.y, 100, 120, bodyType::STATIC);
	pbody->ctype = ColliderType::PLANT;
	pbody->listener = this;

	return true;
}

bool Plant::Update(float dt)
{
	Player* player;
	player = app->entityManager->GetPlayerEntity();

	// L07 DONE 4: Add a physics to an food - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 8;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 8;

	app->render->DrawTexture(texture, position.x - 40, position.y - 40);

	if (touchingPlant) {
		app->render->DrawTexture(texture1, position.x - 40, position.y - 50);
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			//TODO: Destroy collider
			app->tex->UnLoad(texture);
			app->tex->UnLoad(texture1);

			switch (type) {
			case 1:
				player->healPlantCounter++;
				break;
			case 2:
				player->veloPlantCounter++;
				break;
			case 3:
				player->energyPlantCounter++;
				break;
			}
		}
	}



	return true;
}

bool Plant::CleanUp()
{
	app->physics->DestroyBody(pbody);
	return true;
}

void Plant::OnCollision(PhysBody* physA, PhysBody* physB) {
	
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		touchingPlant = true; 

	}
}

void Plant::EndCollision(PhysBody* physA, PhysBody* physB)
{

	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		touchingPlant = false;
		break;
	}
}
