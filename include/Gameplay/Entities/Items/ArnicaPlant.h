#ifndef ARNICAPLANT_H
#define ARNICAPLANT_H

#include "Gameplay/Entities/Items/Plant.h"
#include "Utils/Log.h"

class ArnicaPlant : public Plant {
public:
    // Constructor
    ArnicaPlant(std::string name, int quantity, std::string description);

    // Destructor
    ~ArnicaPlant();

    // Use the arnica plant
    void Use() override;

    bool Update(float dt) override;

    void OnCollision(PhysBody* physA, PhysBody* physB) override;

	void EndCollision(PhysBody* physA, PhysBody* physB) override;

};

#endif // ARNICAPLANT_H