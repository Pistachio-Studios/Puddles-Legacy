#include "Gameplay/Entities/Items/VitaPotion.h"
#include "Gameplay/Entities/Player.h"
#include "Utils/Log.h"
#include "Core/App.h"
#include "Core/EntityManager.h"
#include "Core/Audio.h"

// Constructor
VitaPotion::VitaPotion(std::string name, int quantity, std::string description) 
    : Potion(name, quantity, description) {}

// Destructor
VitaPotion::~VitaPotion() {}

// Use the vita potion
void VitaPotion::Use() {
    // Implement the logic for using the vita potion
    Player* player;
    LOG("Using ", name, ". ", description);
    if(quantity > 0)
    {
        Player* player = app->entityManager->GetPlayerEntity();
        app->audio->PlayFx(player->potionFx);
        if(player != nullptr and player->vida < player->maxVida)
        {
            player->vida++;

            // TODO curar vida
        }
        quantity--;
    }
}