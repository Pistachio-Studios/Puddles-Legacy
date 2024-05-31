#include "Gameplay/Entities/Items/ComfreyPlant.h"
#include "Utils/Log.h"

// Constructor
ComfreyPlant::ComfreyPlant(std::string name, int quantity, std::string description) 
    : Potion(name, quantity, description) {}

// Destructor
ComfreyPlant::~ComfreyPlant() {}

// Use the ComfreyPlant
void ComfreyPlant::Use() {
    // Implement the logic for using the ComfreyPlant
    LOG("Using ", name, ". ", description);
    quantity--;
}