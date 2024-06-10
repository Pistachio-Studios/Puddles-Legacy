#include "Gameplay/Entities/Items/WitchHazelPlant.h"
#include "Utils/Log.h"
#include "Gameplay/Entities/Player.h"
#include "Core/App.h"
#include "Core/Render.h"

// Constructor
WitchHazelPlant::WitchHazelPlant(std::string name, int quantity, std::string description)
    : Plant(name, quantity, description) {}

// Destructor
WitchHazelPlant::~WitchHazelPlant() {}

// Use the HepaticaPlant
void WitchHazelPlant::Use() {
    // Implement the logic for using the WitchHazelPlant
    LOG("Using ", name, ". ", description);
    quantity--;
}

bool WitchHazelPlant::Update(float dt)
{

    position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 8;
    position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 8;

    app->render->DrawTexture(texture, position.x - 40, position.y - 40);

    if (isColliding) {
        app->render->DrawTexture(texture1, position.x - 40, position.y - 90);
        if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
            app->entityManager->GetPlayerEntity()->inventory.AddItem("Witch Hazel Plant");
            app->entityManager->GetPlayerEntity()->bestiary->witchhazelPlantCollected = true;
            app->entityManager->DestroyEntity(this);
            isColliding = false;
        }
    }
    return true;
}

void WitchHazelPlant::OnCollision(PhysBody* physA, PhysBody* physB) {

    switch (physB->ctype) {
    case ColliderType::PLAYER:
        isColliding = true;
        break;

    }
}

void WitchHazelPlant::EndCollision(PhysBody* physA, PhysBody* physB) {
    isColliding = false;
}