#ifndef HEPATICAPLANT_H
#define HEPATICAPLANT_H

#include "Gameplay/Entities/Items/Plant.h"

class HepaticaPlant : public Plant {
public:
    // Constructor
    HepaticaPlant(std::string name, int quantity, std::string description) 
        : Plant(name, quantity, description) {}

    // Destructor
    ~HepaticaPlant() {}

    // Use the hepatica plant
    void Use() override {
        // Implement the logic for using the hepatica plant
        LOG("Using ", name, ". ", description);
        quantity--;
    }
};

#endif // HEPATICAPLANT_H