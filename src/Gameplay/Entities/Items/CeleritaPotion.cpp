#include "Gameplay/Entities/Items/CeleritaPotion.h"
#include "Utils/Log.h"
#include "Gameplay/Entities/Player.h"

// Constructor
CeleritaPotion::CeleritaPotion(std::string name, int quantity, std::string description) 
    : Potion(name, quantity, description) {}

bool CeleritaPotion::Start() {
    timer = Timer();

    return true;
}
// Destructor
CeleritaPotion::~CeleritaPotion() {}

// Use the CeleritaPotion
void CeleritaPotion::Use() {
    // Implement the logic for using the CeleritaPotion
    Player* player; 
    LOG("Using ", name, ". ", description);
    if(quantity > 0)
    {
        quantity--;
        player->maxSpeed *= 2;
        timer.Start();

        if (timer.ReadSec() >= potionDuration) {
            player->maxSpeed /= 2; // Restore original speed after duration
        }
    }
}