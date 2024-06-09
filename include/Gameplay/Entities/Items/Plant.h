#ifndef PLANT_H
#define PLANT_H

#include "Gameplay/Entities/Items/Item.h"

#include "Core/Animation.h"
#include "Core/Physics.h"
#include "Utils/Timer.h"
#include "Utils/StateMachine.h"
//#include "Point.h"
#ifdef __linux__
#include <SDL.h>
#elif _MSC_VER
#include "SDL.h"
#endif

class Plant : public Item {
public:
    // Constructor
    Plant(std::string name, int quantity, std::string description) 
        : Item(EntityType::PLANT, name, quantity, description) {}

    // Destructor
    virtual ~Plant() {}

    bool Start() override;

	bool Update(float dt) override;
	
	bool CleanUp() override;

    // Use the plant
    virtual void Use() = 0; // Declare Use as pure virtual

    void OnCollision(PhysBody* physA, PhysBody* physB) override;

    void EndCollision(PhysBody* physA, PhysBody* physB) override;

public:
    SDL_Texture* texture1 = nullptr;
	SDL_Texture* texture = nullptr;
	const char* texturePath;

    bool isColliding = false;
    PhysBody* pbody;
	
    Item* item; 
};

#endif // PLANT_H