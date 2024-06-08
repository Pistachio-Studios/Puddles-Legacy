#include "Gameplay/Entities/Items/VitaPotion.h"
#include "Gameplay/Entities/Player.h"
#include "Utils/Log.h"

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
    quantity--;
    player->vida = player->totalLivesPlayer;
}