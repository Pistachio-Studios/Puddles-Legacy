#ifndef HAWTHORNPLANT_H
#define HAWTHORNPLANT_H

#include "Gameplay/Entities/Items/Plant.h"
#include "Utils/Log.h"

class HawthornPlant : public Plant {
public:
    // Constructor
    HawthornPlant(std::string name, int quantity, std::string description);

    // Destructor
    ~HawthornPlant();

    // Use the hepatica plant
    void Use() override;

    bool Update(float dt) override;

    void OnCollision(PhysBody* physA, PhysBody* physB) override;

    void EndCollision(PhysBody* physA, PhysBody* physB) override;
};

#endif // HAWTHORNPLANT_H