#include "Gameplay/Entities/Items/EtherPotion.h"
#include "Utils/Log.h"

// Constructor
EtherPotion::EtherPotion(std::string name, int quantity, std::string description) 
    : Potion(name, quantity, description) {}

// Destructor
EtherPotion::~EtherPotion() {}

// Use the EtherPotion
void EtherPotion::Use() {
    // Implement the logic for using the EtherPotion
    LOG("Using ", name, ". ", description);
    quantity--;
}