#ifndef OBLITIUSPOTION_H
#define OBLITIUSPOTION_H

#include "Gameplay/Entities/Items/Potion.h"

class OblitiusPotion : public Potion {
public:
    // Constructor
    OblitiusPotion(std::string name, int quantity, std::string description) 
        : Potion(name, quantity, description) {}

    // Destructor
    ~OblitiusPotion() {}

    // Use the Oblitius potion
    void Use() override {
        // Implement the logic for using the Oblitius potion
        LOG("Using ", name, ". ", description);
        quantity--;
    }
};

#endif // OBLITIUSPOTION_H