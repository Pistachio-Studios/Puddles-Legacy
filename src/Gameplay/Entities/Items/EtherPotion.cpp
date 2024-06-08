#include "Gameplay/Entities/Items/EtherPotion.h"
#include "Utils/Log.h"
#include "Core/App.h"
#include "Core/EntityManager.h"
#include "Gameplay/Entities/Player.h"

// Constructor
EtherPotion::EtherPotion(std::string name, int quantity, std::string description) 
    : Potion(name, quantity, description) {}

// Destructor
EtherPotion::~EtherPotion() {}

// Use the EtherPotion
void EtherPotion::Use() {
    // Implement the logic for using the EtherPotion
    LOG("Using ", name, ". ", description);
    if(quantity > 0)
    {
        Player* player = app->entityManager->GetPlayerEntity();
        if(player != nullptr and player->mana < 100)
        {
            player->mana = 100.0f;
        }
        quantity--;
    }
}