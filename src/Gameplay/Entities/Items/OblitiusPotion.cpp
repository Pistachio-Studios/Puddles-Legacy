#include "Gameplay/Entities/Items/OblitiusPotion.h"
#include "Core/Audio.h"
#include "Gameplay/Entities/Player.h"
#include "Utils/Log.h"

// Constructor
OblitiusPotion::OblitiusPotion(std::string name, int quantity, std::string description) 
    : Potion(name, quantity, description) {}

// Destructor
OblitiusPotion::~OblitiusPotion() {}

// Use the Oblitius potion
void OblitiusPotion::Use() {
    // Implement the logic for using the Oblitius potion
    Player* player = app->entityManager->GetPlayerEntity();
    LOG("Using ", name, ". ", description);
    if (quantity > 0) {
        quantity--;
        app->audio->PlayFx(player->potionFx);

        player->bestiary->swordAbility100Unlocked = false;
        player->bestiary->swordAbility110Unlocked = false;
        player->bestiary->swordAbility111Unlocked = false;
        player->bestiary->swordAbility112Unlocked = false;
        player->bestiary->swordAbility120Unlocked = false;
        player->bestiary->swordAbility122Unlocked = false;
        player->bestiary->swordAbility123Unlocked = false;

        player->bestiary->staffAbility100Unlocked = false;
        player->bestiary->staffAbility110Unlocked = false;
        player->bestiary->staffAbility111Unlocked = false;
        player->bestiary->staffAbility112Unlocked = false;
        player->bestiary->staffAbility120Unlocked = false;
        player->bestiary->staffAbility122Unlocked = false;
        player->bestiary->staffAbility123Unlocked = false;
    }
}