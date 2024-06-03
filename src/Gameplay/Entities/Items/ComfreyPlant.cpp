#include "Gameplay/Entities/Items/ComfreyPlant.h"
#include "Utils/Log.h"
#include "Gameplay/Entities/Player.h"
#include "Core/App.h"
#include "Core/Render.h"

// Constructor
ComfreyPlant::ComfreyPlant(std::string name, int quantity, std::string description) 
    : Plant(name, quantity, description) {}

// Destructor
ComfreyPlant::~ComfreyPlant() {}

// Use the ComfreyPlant
void ComfreyPlant::Use() {
    // Implement the logic for using the ComfreyPlant
    LOG("Using ", name, ". ", description);
    quantity--;
    counter--;
}

bool ComfreyPlant::Update(float dt)
{

    position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 8;
    position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 8;

    app->render->DrawTexture(texture, position.x - 40, position.y - 40);

    if (isColliding) {
        app->render->DrawTexture(texture1, position.x - 40, position.y - 50);
        if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
            counter++;
            app->entityManager->GetPlayerEntity()->inventory.AddItem("Comfrey Plant");
            app->entityManager->DestroyEntity(this);
            isColliding = false;
        }
    }
    return true;
}

void ComfreyPlant::OnCollision(PhysBody *physA, PhysBody *physB) {

    switch (physB->ctype) {
	    case ColliderType::PLAYER:
            isColliding = true;
            break;
            
	}
}

void ComfreyPlant::EndCollision(PhysBody *physA, PhysBody *physB) {
    isColliding = false;
}
