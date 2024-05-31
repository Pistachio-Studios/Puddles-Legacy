#ifndef ETHERPOTION_H
#define ETHERPOTION_H

#include "Gameplay/Entities/Items/Potion.h"

class EtherPotion : public Potion {
public:
    // Constructor
    EtherPotion(std::string name, int quantity, std::string description);

    // Destructor
    ~EtherPotion();

    // Use the ether potion
    void Use() override;
};

#endif // ETHERPOTION_H