#include "Gameplay/Entities/Items/Plant.h"
#include "Gameplay/Entities/Player.h"
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

// Start the plant
bool Plant::Start() {

    position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	texture = app->tex->Load(texturePath);
	texture1 = app->tex->Load("Assets/Textures/pressE.png");

	pbody = app->physics->CreateRectangle(position.x, position.y, 100, 120, bodyType::STATIC);
	pbody->ctype = ColliderType::PLANT;
	pbody->listener = this;

    return true;
}

// Update the plant
bool Plant::Update(float dt) {

    return true;
}

// Clean up the plant
bool Plant::CleanUp() {
    app->tex->UnLoad(texture);
    app->tex->UnLoad(texture1);
    app->physics->DestroyBody(pbody);
    return true;
}

void Plant::OnCollision(PhysBody *physA, PhysBody *physB) {
}

void Plant::EndCollision(PhysBody *physA, PhysBody *physB) {
}
