#ifndef PLANT_H
#define PLANT_H

#include "Gameplay/Entities/Items/Item.h"

class Plant : public Item {
public:
    // Constructor
    Plant(std::string name, int quantity, std::string description) 
        : Item(EntityType::PLANT, name, quantity, description) {}

    // Destructor
    virtual ~Plant() {}

    // Use the plant
    virtual void Use() = 0; // Declare Use as pure virtual
};

#endif // PLANT_H