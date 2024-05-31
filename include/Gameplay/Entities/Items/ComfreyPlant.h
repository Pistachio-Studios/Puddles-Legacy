#ifndef COMFREYPLANT_H
#define COMFREYPLANT_H

#include "Gameplay/Entities/Items/Plant.h"

class ComfreyPlant : public Plant {
public:
    // Constructor
    ComfreyPlant(std::string name, int quantity, std::string description) 
        : Plant(name, quantity, description) {}

    // Destructor
    ~ComfreyPlant() {}

    // Use the Comfrey plant
    void Use() override {
        // Implement the logic for using the Comfrey plant
        LOG("Using ", name, ". ", description);
        quantity--;
    }
};

#endif // COMFREYPLANT_H