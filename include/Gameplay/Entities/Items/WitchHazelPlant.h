#ifndef WITCHHAZELPLANT_H
#define WITCHHAZELPLANT_H

#include "Gameplay/Entities/Items/Plant.h"
#include "Utils/Log.h"

class WitchHazelPlant : public Plant {
public:
    // Constructor
    WitchHazelPlant(std::string name, int quantity, std::string description);

    // Destructor
    ~WitchHazelPlant();

    // Use the hepatica plant
    void Use() override;

    bool Update(float dt) override;

    void OnCollision(PhysBody* physA, PhysBody* physB) override;

    void EndCollision(PhysBody* physA, PhysBody* physB) override;
};

#endif // WITCHHAZELPLANT_H