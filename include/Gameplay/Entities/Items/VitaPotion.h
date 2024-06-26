#ifndef VITAPOTION_H
#define VITAPOTION_H

#include "Potion.h"

class VitaPotion : public Potion {
public:
    // Constructor
    VitaPotion(std::string name, int quantity, std::string description);

    // Destructor
    ~VitaPotion();

    // Use the vita potion
    void Use() override;
};

#endif // VITAPOTION_H