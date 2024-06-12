#include "Gameplay/Entities/Items/CeleritaPotion.h"
#include "Utils/Log.h"
#include "Gameplay/Entities/Player.h"
#include "Core/App.h"
#include "Core/Audio.h"
#include "Core/EntityManager.h"

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
    Player* player = app->entityManager->GetPlayerEntity(); 
    LOG("Using ", name, ". ", description);
    if(quantity > 0 and player != nullptr)
    {

        app->audio->PlayFx(player->potionFx);
        quantity--;
        if(player->maxSpeed <= 10)player->maxSpeed *= 2;
        if(player->moveForce <= 2)player->moveForce *= 2;
        timer.Start();
    }
}

bool CeleritaPotion::Update(float dt) {
   if(quantity >= 0 and timer.ReadSec() >= potionDuration)
   {
       Player* player = app->entityManager->GetPlayerEntity();
       if(player != nullptr)
       {
           if(player->maxSpeed >= 10)player->maxSpeed /= 2;
           if(player->moveForce >= 2)player->moveForce /= 2;
       }
   }
   return true;
}