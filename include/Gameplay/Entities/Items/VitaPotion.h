#ifndef VITAPOTION_H
#define VITAPOTION_H

#include "Potion.h"

class VitaPotion : public Potion {
public:
    // Constructor
    VitaPotion(std::string name, int quantity, std::string description) 
        : Potion(name, quantity, description) {}

    // Destructor
    ~VitaPotion() {}

    // Use the vita potion
    void Use() override {
        // Implement the logic for using the vita potion
        LOG("Using ", name, ". ", description);
        quantity--;
    }
};

#endif // VITAPOTION_H