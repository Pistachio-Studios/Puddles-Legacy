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
	texturePath = parameters.attribute("texturepath").as_string();

	texture = app->tex->Load(texturePath);

	pbody = app->physics->CreateRectangle(position.x, position.y, 80, 80, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::POTION;
	pbody->listener = this;

	return true;
}

bool Potion::Update(float dt)
{
	// L07 DONE 4: Add a physics to an food - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 8;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 8;

	//TODO: Cada pocion hace su cosa

	if (isCreated) {
		//TODO: Dibujar la textura de la pocion
		app->render->DrawTexture(texture, position.x - 10, position.y - 10); 
	}
	else if (isCreated == false) {
		app->tex->UnLoad(texture);
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