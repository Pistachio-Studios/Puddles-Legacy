#include "Gameplay/Entities/Items/OblitiusPotion.h"
#include "Utils/Log.h"

// Constructor
OblitiusPotion::OblitiusPotion(std::string name, int quantity, std::string description) 
    : Potion(name, quantity, description) {}

// Destructor
OblitiusPotion::~OblitiusPotion() {}

// Use the Oblitius potion
void OblitiusPotion::Use() {
    // Implement the logic for using the Oblitius potion
    LOG("Using ", name, ". ", description);
    quantity--;
}