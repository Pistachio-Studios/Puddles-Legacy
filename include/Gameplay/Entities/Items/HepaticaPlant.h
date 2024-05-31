#ifndef HEPATICAPLANT_H
#define HEPATICAPLANT_H

#include "Gameplay/Entities/Items/Plant.h"
#include "Utils/Log.h"

class HepaticaPlant : public Plant {
public:
    // Constructor
    HepaticaPlant(std::string name, int quantity, std::string description);

    // Destructor
    ~HepaticaPlant();

    // Use the hepatica plant
    void Use() override;
};

#endif // HEPATICAPLANT_H