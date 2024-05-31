#ifndef OBLITIUSPOTION_H
#define OBLITIUSPOTION_H

#include "Gameplay/Entities/Items/Potion.h"

class OblitiusPotion : public Potion {
public:
    // Constructor
    OblitiusPotion(std::string name, int quantity, std::string description);

    // Destructor
    ~OblitiusPotion();

    // Use the Oblitius potion
    void Use() override;
};

#endif // OBLITIUSPOTION_H