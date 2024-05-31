#include "Gameplay/Entities/Items/HepaticaPlant.h"
#include "Utils/Log.h"

// Constructor
HepaticaPlant::HepaticaPlant(std::string name, int quantity, std::string description) 
    : Potion(name, quantity, description) {}

// Destructor
HepaticaPlant::~HepaticaPlant() {}

// Use the HepaticaPlant
void HepaticaPlant::Use() {
    // Implement the logic for using the HepaticaPlant
    LOG("Using ", name, ". ", description);
    quantity--;
}