#include "Gameplay/Entities/FoodItem.h"
#include "Core/App.h"
#include "Core/Textures.h"
#include "Core/Audio.h"
#include "Core/Input.h"
#include "Core/Render.h"
#include "Gameplay/Scene.h"
#include "Utils/Log.h"
#include "Utils/Point.h"
#include "Core/Physics.h"
#include "Core/Map.h"
#include <cstdlib>

#include <box2d/b2_body.h>

FoodItem::FoodItem() : Entity(EntityType::FOODITEM)
{
	name.Create("food");
}

FoodItem::~FoodItem() {}

bool FoodItem::Awake() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool FoodItem::Start() {

	// Initialize textures
	foodTextures = *app->map->GetAnimByName("Food");

	// Pick a random texture
	int randomIndex = rand() % foodTextures.totalFrames;
	foodTextures.currentFrame = randomIndex;
	foodTextures.loop = true;

	pbody = app->physics->CreateCircle(position.x + 8, position.y + 8, 8, bodyType::STATIC);
	pbody->ctype = ColliderType::FOOD;
	pbody->listener = this;

	return true;
}

bool FoodItem::Update(float dt)
{
	// L07 DONE 4: Add a physics to an food - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 8;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 8;

	app->render->DrawTexture(foodTextures.texture, position.x, position.y, &foodTextures.GetCurrentFrame());

	return true;
}

bool FoodItem::CleanUp()
{
	app->physics->DestroyBody(pbody);
	return true;
}

void FoodItem::OnCollision(PhysBody* physA, PhysBody* physB)
{
	if (physB->ctype == ColliderType::PLAYER){
		if(app->scene->player->lives < 7) app->scene->player->lives++;
		app->entityManager->DestroyEntity(this);
	}
}