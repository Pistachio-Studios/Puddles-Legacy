#include "Gameplay/Entities/Items/ArnicaPlant.h"
#include "Utils/Log.h"

// Constructor
ArnicaPlant::ArnicaPlant(std::string name, int quantity, std::string description) 
    : Potion(name, quantity, description) {}

// Destructor
ArnicaPlant::~ArnicaPlant() {}

// Use the ArnicaPlant
void ArnicaPlant::Use() {
    // Implement the logic for using the ArnicaPlant
    LOG("Using ", name, ". ", description);
    quantity--;
}