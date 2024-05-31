#ifndef ARNICAPLANT_H
#define ARNICAPLANT_H

#include "Gameplay/Entities/Items/Plant.h"

class ArnicaPlant : public Plant {
public:
    // Constructor
    ArnicaPlant(std::string name, int quantity, std::string description) 
        : Plant(name, quantity, description) {}

    // Destructor
    ~ArnicaPlant() {}

    // Use the arnica plant
    void Use() override {
        // Implement the logic for using the arnica plant
        LOG("Using ", name, ". ", description);
        quantity--;
    }
};

#endif // ARNICAPLANT_H