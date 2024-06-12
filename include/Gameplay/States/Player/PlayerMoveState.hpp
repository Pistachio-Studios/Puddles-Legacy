#ifndef __PLAYERMOVESTATE_H__
#define __PLAYERMOVESTATE_H__

#include "Gameplay/Entities/Player.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"

class PlayerMoveState : public State<Player> {
private:
    Player* player;

public:
    PlayerMoveState(SString name) : State(name) {}
    inline void Enter() override
    {
        GamePad& pad = app->input->pads[0];

        player = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        GamePad& pad = app->input->pads[0];

        if (player->timerSteps >= player->time)
        {
            int randomIndex = rand() % 4; // Genera un índice aleatorio entre 0 y 3
            player->currentFootstepFx = player->footstepFx[randomIndex];

            // Reproduce el sonido
            app->audio->PlayFx(player->currentFootstepFx);
            player->timerSteps = 0.0f;
        }

        PhysBody* pbody = player->pbody;

        player->flip = SDL_FLIP_NONE;

        b2Vec2 impulse = { 0, 0 };

        if (pbody->body->GetLinearVelocity().Length() < player->maxSpeed)
        {
            if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || pad.l_x < 0.0f) {
                impulse.x = -pbody->body->GetMass() * player->moveForce;
                if (player->currentClass == KNIGHT) {
                    //Animation
                    player->SabrinaEspadaMovIzquierda.Update(dt);
                    player->currentAnim = &player->SabrinaEspadaMovIzquierda;
                }
                if (player->currentClass == WIZARD) {
                    //Animation
                    player->SabrinaCetroMovIzquierda.Update(dt);
                    player->currentAnim = &player->SabrinaCetroMovIzquierda;
                }
            }
            if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || pad.l_x > 0.0f) {
                impulse.x = pbody->body->GetMass() * player->moveForce;
                //Animation
                if (player->currentClass == KNIGHT) {
                    player->SabrinaEspadaMovDerecha.Update(dt);
                    player->currentAnim = &player->SabrinaEspadaMovDerecha;
                }
                if (player->currentClass == WIZARD) {
                    player->SabrinaCetroMovDerecha.Update(dt);
                    player->currentAnim = &player->SabrinaCetroMovDerecha;
                }                
            }
            if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || pad.l_y < 0.0f) {
               impulse.y = -pbody->body->GetMass() * player->moveForce;
               //Animation
               if (player->currentClass == KNIGHT) {
                   player->SabrinaEspadaMovDetras.Update(dt);
                   player->currentAnim = &player->SabrinaEspadaMovDetras;
               }
               if (player->currentClass == WIZARD) {
                   player->SabrinaCetroMovDetras.Update(dt);
                   player->currentAnim = &player->SabrinaCetroMovDetras;
               }
            }
            if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || pad.l_y > 0.0f) {
                impulse.y = pbody->body->GetMass() * player->moveForce;                
                //Animation
                if (player->currentClass == KNIGHT) {
                    player->SabrinaEspadaMovDelante.Update(dt);
                    player->currentAnim = &player->SabrinaEspadaMovDelante;
                }
                if (player->currentClass == WIZARD) {
                    player->SabrinaCetroMovDelante.Update(dt);
                    player->currentAnim = &player->SabrinaCetroMovDelante;
                }
            }

            pbody->body->ApplyLinearImpulse(impulse, pbody->body->GetWorldCenter(), true);
        }

        if(app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN and player->dashTimer.ReadSec() > player->dashCultdown)
        {
            b2Vec2 dir = pbody->body->GetLinearVelocity();
            dir.Normalize();

            float dashForce = 100.0f;
            app->audio->PlayFx(player->dashFx);
            b2Vec2 impulse = { dir.x * pbody->body->GetMass() * player->moveForce * dashForce, dir.y * pbody->body->GetMass() * player->moveForce * dashForce};

            pbody->body->ApplyLinearImpulse(impulse, pbody->body->GetWorldCenter(), true);

            player->dashTimer.Start();

            //Animation
            if (player->currentClass == KNIGHT) {
                player->SabrinaEspadaDash.Update(dt);
                player->currentAnim = &player->SabrinaEspadaDash;
            }
            if (player->currentClass == WIZARD) {
                player->SabrinaCetroDash.Update(dt);
                player->currentAnim = &player->SabrinaCetroDash;
            }
        }

        if (pbody->body->GetLinearVelocity().Length() < 1.0f)
        {
            StateMachineReference->ChangeState("idle");
        }
    }
    inline void Exit() override
    {

    }
};
#endif // __PLAYERMOVESTATE_H__