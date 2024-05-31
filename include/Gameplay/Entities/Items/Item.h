#ifndef ITEM_H
#define ITEM_H

#include "Gameplay/Entities/Entity.h"

class Item : public Entity {
public:
    // Constructor
    Item(EntityType type, std::string name, int quantity, std::string description) 
        : Entity(type), name(name), quantity(quantity), description(description) {}

    // Destructor
    ~Item() {}

    // Use the item
    virtual void Use() = 0; // This will be implemented in the derived classes

    // Get the item type
    EntityType GetType() const {
        return type;
    }

public:
    std::string name;
    int quantity;
    std::string description;
};

#endif // ITEM_H