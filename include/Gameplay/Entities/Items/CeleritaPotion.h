#ifndef CELERITAPOTION_H
#define CELERITAPOTION_H

#include "Gameplay/Entities/Items/Potion.h"

class CeleritaPotion : public Potion {
public:
    // Constructor
    CeleritaPotion(std::string name, int quantity, std::string description);

    // Destructor
    ~CeleritaPotion();

    // Use the celerita potion
    void Use() override;


};

#endif // CELERITAPOTION_H