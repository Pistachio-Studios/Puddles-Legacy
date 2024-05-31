#ifndef COMFREYPLANT_H
#define COMFREYPLANT_H

#include "Gameplay/Entities/Items/Plant.h"
#include "Utils/Log.h"

class ComfreyPlant : public Plant {
public:
    // Constructor
    ComfreyPlant(std::string name, int quantity, std::string description);

    // Destructor
    ~ComfreyPlant();

    // Use the Comfrey plant
    void Use() override;
};

#endif // COMFREYPLANT_H