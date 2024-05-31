#ifndef CELERITAPOTION_H
#define CELERITAPOTION_H

#include "Gameplay/Entities/Items/Potion.h"

class CeleritaPotion : public Potion {
public:
    // Constructor
    CeleritaPotion(std::string name, int quantity, std::string description) 
        : Potion(name, quantity, description) {}

    // Destructor
    ~CeleritaPotion() {}

    // Use the celerita potion
    void Use() override {
        // Implement the logic for using the celerita potion
        LOG("Using ", name, ". ", description);
        quantity--;
    }
};

#endif // CELERITAPOTION_H