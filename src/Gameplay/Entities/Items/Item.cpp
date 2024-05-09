#include "Gameplay/Entities/Items/Item.h"

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

#ifdef __linux__
#include <Box2D/Dynamics/b2Body.h>
#endif

Item::Item() : Entity(EntityType::ITEM)
{
	name.Create("item");
}

Item::~Item() {}

bool Item::Awake() {

	return true;
}

bool Item::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	texture = app->tex->Load(texturePath);
	texture1 = app->tex->Load("Assets/Textures/pressE.png");

	pbody = app->physics->CreateRectangle(position.x, position.y, 80, 80, bodyType::STATIC);
	pbody->ctype = ColliderType::ITEM;
	pbody->listener = this;

	return true;
}

bool Item::Update(float dt)
{
	// L07 DONE 4: Add a physics to an food - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 8;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 8;
	
	if (touchingItem) {
		app->render->DrawTexture(texture1, position.x - 40, position.y - 20);
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
			//TODO: Destroy collider
			app->tex->UnLoad(texture); 
			app->tex->UnLoad(texture1);
			isPicked = true; 
		}
	}

	app->render->DrawTexture(texture, position.x - 10, position.y - 10);

	return true;
}

bool Item::CleanUp()
{
	app->physics->DestroyBody(pbody);
	return true;
}

void Item::OnCollision(PhysBody* physA, PhysBody* physB) {
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		touchingItem = true;
		break;
	}
}

void Item::EndCollision(PhysBody* physA, PhysBody* physB)
{
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		touchingItem = false;
		break;
	}
}