#include "Gameplay/Entities/Items/CeleritaPotion.h"
#include "Utils/Log.h"

// Constructor
CeleritaPotion::CeleritaPotion(std::string name, int quantity, std::string description) 
    : Potion(name, quantity, description) {}

// Destructor
CeleritaPotion::~CeleritaPotion() {}

// Use the CeleritaPotion
void CeleritaPotion::Use() {
    // Implement the logic for using the CeleritaPotion
    LOG("Using ", name, ". ", description);
    if(quantity > 0)quantity--;
}