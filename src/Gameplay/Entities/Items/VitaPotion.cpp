#include "Gameplay/Entities/Items/VitaPotion.h"
#include "Utils/Log.h"
#include "Core/App.h"
#include "Core/EntityManager.h"
#include "Gameplay/Entities/Player.h"

// Constructor
VitaPotion::VitaPotion(std::string name, int quantity, std::string description) 
    : Potion(name, quantity, description) {}

// Destructor
VitaPotion::~VitaPotion() {}

// Use the vita potion
void VitaPotion::Use() {
    // Implement the logic for using the vita potion
    LOG("Using ", name, ". ", description);
    if(quantity > 0)
    {
        Player* player = app->entityManager->GetPlayerEntity();
        if(player != nullptr and player->livesPlayer < player->totalLivesPlayer)
        {
            player->livesPlayer++;
        }
        quantity--;
    }
}