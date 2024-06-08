#include "Gameplay/Entities/Items/ArnicaPlant.h"
#include "Utils/Log.h"
#include "Core/App.h"
#include "Gameplay/Entities/Player.h"
#include "Core/Render.h"
#include "Core/Physics.h"

// Constructor
ArnicaPlant::ArnicaPlant(std::string name, int quantity, std::string description) 
    : Plant(name, quantity, description) {}

// Destructor
ArnicaPlant::~ArnicaPlant() {}

// Use the ArnicaPlant
void ArnicaPlant::Use() {
    // Implement the logic for using the ArnicaPlant
    LOG("Using ", name, ". ", description);
    quantity--;
    //TODO: Si se le da al boton de accept en la tavernscene hay que restar 3 plantas del inventario (si se le da a la oblitius potion se le resta solo 1 planta de cada)
}

bool ArnicaPlant::Update(float dt)
{

    position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 8;
    position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 8;

    app->render->DrawTexture(texture, position.x - 40, position.y - 40);

    if (isColliding) {
        app->render->DrawTexture(texture1, position.x - 40, position.y - 50);
        if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
            app->entityManager->GetPlayerEntity()->inventory.AddItem("Arnica Plant");
            app->entityManager->GetPlayerEntity()->bestiary->arnicaPlantCollected = true;
            app->entityManager->DestroyEntity(this);
            isColliding = false;
        }
    }
    return true;
}

void ArnicaPlant::OnCollision(PhysBody *physA, PhysBody *physB) {

    switch (physB->ctype) {
	    case ColliderType::PLAYER:
            isColliding = true;
            break;
            
	}
}

void ArnicaPlant::EndCollision(PhysBody *physA, PhysBody *physB) {
    isColliding = false;
}