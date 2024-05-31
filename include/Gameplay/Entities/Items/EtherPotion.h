#ifndef ETHERPOTION_H
#define ETHERPOTION_H

#include "Gameplay/Entities/Items/Potion.h"

class EtherPotion : public Potion {
public:
    // Constructor
    EtherPotion(std::string name, int quantity, std::string description) 
        : Potion(name, quantity, description) {}

    // Destructor
    ~EtherPotion() {}

    // Use the ether potion
    void Use() override {
        // Implement the logic for using the ether potion
        LOG("Using ", name, ". ", description);
        quantity--;
    }
};

#endif // ETHERPOTION_H