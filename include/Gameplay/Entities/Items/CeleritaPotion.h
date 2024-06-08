#ifndef CELERITAPOTION_H
#define CELERITAPOTION_H

#include "Gameplay/Entities/Items/Potion.h"
#include "Utils/Timer.h"
class CeleritaPotion : public Potion {
public:
    // Constructor
    CeleritaPotion(std::string name, int quantity, std::string description);

    // Destructor
    ~CeleritaPotion();

    bool Start();

    // Use the celerita potion
    void Use() override;

    bool Update(float dt) override;

public:
    bool startTimer = true;
    Timer timer;

    const float potionDuration = 5.0f; // Duration of the potion effect in seconds
};

#endif // CELERITAPOTION_H