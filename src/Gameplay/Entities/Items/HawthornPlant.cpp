#include "Gameplay/Entities/Items/HawthornPlant.h"
#include "Utils/Log.h"
#include "Gameplay/Entities/Player.h"
#include "Core/App.h"
#include "Core/Render.h"

// Constructor
HawthornPlant::HawthornPlant(std::string name, int quantity, std::string description)
    : Plant(name, quantity, description) {}

// Destructor
HawthornPlant::~HawthornPlant() {}

// Use the HepaticaPlant
void HawthornPlant::Use() {
    // Implement the logic for using the WitchHazelPlant
    LOG("Using ", name, ". ", description);
    quantity--;
}

bool HawthornPlant::Update(float dt)
{

    position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 8;
    position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 8;

    app->render->DrawTexture(texture, position.x - 40, position.y - 40);

    if (isColliding) {
        app->render->DrawTexture(texture1, position.x - 40, position.y - 50);
        if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
            app->entityManager->GetPlayerEntity()->inventory.AddItem("Hawthorn Plant");
            app->entityManager->GetPlayerEntity()->bestiary->hawthornPlantCollected = true;
            app->entityManager->DestroyEntity(this);
            isColliding = false;
        }
    }
    return true;
}

void HawthornPlant::OnCollision(PhysBody* physA, PhysBody* physB) {

    switch (physB->ctype) {
    case ColliderType::PLAYER:
        isColliding = true;
        break;

    }
}

void HawthornPlant::EndCollision(PhysBody* physA, PhysBody* physB) {
    isColliding = false;
}