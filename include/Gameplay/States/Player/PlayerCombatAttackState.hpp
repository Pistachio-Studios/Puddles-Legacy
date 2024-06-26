#ifndef __PLAYERCOMBATATTACKSTATE_H__
#define __PLAYERCOMBATATTACKSTATE_H__

#include "Gameplay/Entities/Player.h"
#include "Gameplay/Entities/Sword.h"
#include "Gameplay/Entities/Staff.h"
#include <Core/Input.h>
#include <Core/App.h>
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"

class PlayerCombatAttackState : public State<Player> {
private:
    Player* player;

    float attackRange;
    float attackValue;
    float playerLookingAngle;
    float attackSpeed;
    float radius;

    bool attacking = false;

    bool firstSpell;

    Timer spellTimer;

public:
    PlayerCombatAttackState(SString name) : State(name) {}
    inline void Enter() override
    {
        player = StateMachineReference->owner;

        attackRange = 150;
        attackValue = -attackRange / 2;
        playerLookingAngle = player->lookingAngle;
        attackSpeed = 500;
        radius = 3.5;
        attacking = false;

        if(player->mana <= 10.0f)
        {
            player->combatFSM->ChangeState("idle");
            return;
        }

        if(player->currentClass == PlayerClass::KNIGHT)
        {
            player->swordEntity->Equip();
        }
        else if(player->currentClass == PlayerClass::WIZARD)
        {
            spellTimer.Start();
            player->staffEntity->Equip();
            firstSpell = true;
        }
    }
    inline void Update(float dt) override
    {
        GamePad& pad = app->input->pads[0];

        if(player->currentClass == PlayerClass::KNIGHT and player->mana > 10.0f)
        {
           
            if (player->timerSword >= player->time)
            {
                app->audio->PlayFx(player->swordSlashFx);
                player->timerSword = 0.0f;
            }

            if (attackValue < attackRange / 2)
            {
                b2Vec2 playerPos = player->pbody->body->GetPosition();
                b2Vec2 swordPos = { playerPos.x + (float)cos(playerLookingAngle + attackValue * DEGTORAD) * radius, playerPos.y + (float)sin(playerLookingAngle + attackValue * DEGTORAD) * radius };
                player->swordEntity->pbody->body->SetTransform(swordPos, playerLookingAngle + attackValue * DEGTORAD);
                attackValue += dt / 1000 * attackSpeed;
                if(player->mana > 5.0f and attacking == false)player->mana -= 5.0f;
                attacking = true;
                //Animation
                player->SabrinaEspadaAtaque.Update(dt);
                if(player->lookingAngle > 4.7f or player->lookingAngle < 1.5f)
                {
                    player->flip = SDL_FLIP_HORIZONTAL;
                }
                else
                {
                    player->flip = SDL_FLIP_NONE;
                }
                player->currentAnim = &player->SabrinaEspadaAtaque;

                player->position.x -= 64;
            }
            else
            {
                StateMachineReference->ChangeState("idle");
            }
        }
        else if(player->currentClass == PlayerClass::WIZARD and player->mana > 10.0f)
        {
            b2Vec2 playerPos = player->pbody->body->GetPosition();
            b2Vec2 staffPos = { playerPos.x + (float)cos(player->lookingAngle) * radius, playerPos.y + (float)sin(player->lookingAngle) * radius };
            player->staffEntity->pbody->body->SetTransform(staffPos, player->lookingAngle);
            player->staffEntity->pbody->GetPosition(player->staffEntity->position.x, player->staffEntity->position.y);

            if (app->input->GetMouseButtonDown(1) or pad.r2 > 0.0f)
            {
                if (player->timerSword >= player->time)
                {
                    app->audio->PlayFx(player->firestaffFx);
                    player->timerSword = 0.0f;
                }
                //Animation
                player->SabrinaCetroAtaque.Update(dt);
                player->currentAnim = &player->SabrinaCetroAtaque;

                if(spellTimer.ReadMSec() > 500 or firstSpell)
                {
                    player->staffEntity->ThrowSpell();
                    spellTimer.Start();
                    firstSpell = false;                    
                    if(player->mana > 10.0f)player->mana -= 10.0f;
                }
            }
            else if(spellTimer.ReadSec() > 5)
            {
                StateMachineReference->ChangeState("idle");
            }
        }
        else
        {
            StateMachineReference->ChangeState("idle");
        }
    }
    inline void Exit() override
    {
        if(player->currentClass == PlayerClass::KNIGHT)
        {
            player->swordEntity->Store();
        }
        else if(player->currentClass == PlayerClass::WIZARD)
        {
            player->staffEntity->Store();
        }
    }
};
#endif // __PlayerCombatAttackState_H__