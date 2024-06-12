#include "Gameplay/Entities/Items/OblitiusPotion.h"
#include "Core/Audio.h"
#include "Gameplay/Entities/Player.h"
#include "Utils/Log.h"

// Constructor
OblitiusPotion::OblitiusPotion(std::string name, int quantity, std::string description) 
    : Potion(name, quantity, description) {}

// Destructor
OblitiusPotion::~OblitiusPotion() {}

// Use the Oblitius potion
void OblitiusPotion::Use() {
    // Implement the logic for using the Oblitius potion
    Player* player = app->entityManager->GetPlayerEntity();
    LOG("Using ", name, ". ", description);
    if (quantity > 0) {
        quantity--;
        app->audio->PlayFx(player->potionFx);

    }
}