#ifndef POTION_H
#define POTION_H

#include "Gameplay/Entities/Items/Item.h"
#include "Utils/Log.h"

class Potion : public Item {
public:
    // Constructor
    Potion(std::string name, int quantity, std::string description) 
        : Item(EntityType::POTION, name, quantity, description) {}

    // Destructor
    virtual ~Potion() {}

    // Use the potion
    virtual void Use() = 0; // Declare Use as pure virtual
};

#endif // POTION_H