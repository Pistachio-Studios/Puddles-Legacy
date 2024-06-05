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


        player = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {

        
        PhysBody* pbody = player->pbody;

        b2Vec2 impulse = { 0, 0 };

        if (pbody->body->GetLinearVelocity().Length() < player->maxSpeed)
        {
            if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
                impulse.x = -pbody->body->GetMass() * player->moveForce;
                if (player->currentClass == KNIGHT) {
                    //Animation
                    app->render->DrawTexture(player->SabrinaEspadaMovDerecha.texture, player->position.x - 15, player->position.y - 25, &player->SabrinaEspadaMovDerecha.GetCurrentFrame());
                    player->SabrinaEspadaMovDerecha.Update(dt);
                }
                if (player->currentClass == WIZARD) {
                    //Animation
                    app->render->DrawTexture(player->SabrinaCetroMovDerecha.texture, player->position.x - 15, player->position.y - 25, &player->SabrinaCetroMovDerecha.GetCurrentFrame());
                    player->SabrinaCetroMovDerecha.Update(dt);
                }
            }
            if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
                impulse.x = pbody->body->GetMass() * player->moveForce;
                //Animation
                if (player->currentClass == KNIGHT) {
                    app->render->DrawTexture(player->SabrinaEspadaMovDerecha.texture, player->position.x - 15, player->position.y - 25, &player->SabrinaEspadaMovDerecha.GetCurrentFrame());
                    player->SabrinaEspadaMovDerecha.Update(dt);
                }
                if (player->currentClass == WIZARD) {
                    app->render->DrawTexture(player->SabrinaCetroMovDerecha.texture, player->position.x - 15, player->position.y - 25, &player->SabrinaCetroMovDerecha.GetCurrentFrame());
                    player->SabrinaCetroMovDerecha.Update(dt);
                }                
            }
            if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
               impulse.y = -pbody->body->GetMass() * player->moveForce;
               //Animation
               if (player->currentClass == KNIGHT) {
                   app->render->DrawTexture(player->SabrinaEspadaMovDetras.texture, player->position.x - 15, player->position.y - 25, &player->SabrinaEspadaMovDetras.GetCurrentFrame());
                   player->SabrinaEspadaMovDetras.Update(dt);
               }
               if (player->currentClass == WIZARD) {
                   app->render->DrawTexture(player->SabrinaCetroMovDetras.texture, player->position.x - 15, player->position.y - 25, &player->SabrinaCetroMovDetras.GetCurrentFrame());
                   player->SabrinaCetroMovDetras.Update(dt);
               }
            }
            if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
                impulse.y = pbody->body->GetMass() * player->moveForce;                
                //Animation
                if (player->currentClass == KNIGHT) {
                    app->render->DrawTexture(player->SabrinaEspadaMovDelante.texture, player->position.x - 15, player->position.y - 25, &player->SabrinaEspadaMovDelante.GetCurrentFrame());
                    player->SabrinaEspadaMovDelante.Update(dt);
                }
                if (player->currentClass == WIZARD) {
                    app->render->DrawTexture(player->SabrinaCetroMovDelante.texture, player->position.x - 15, player->position.y - 25, &player->SabrinaCetroMovDelante.GetCurrentFrame());
                    player->SabrinaCetroMovDelante.Update(dt);
                }
            }

            pbody->body->ApplyLinearImpulse(impulse, pbody->body->GetWorldCenter(), true);
        }

        if(app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN and player->dashTimer.ReadSec() > player->dashCultdown)
        {
            b2Vec2 dir = pbody->body->GetLinearVelocity();
            dir.Normalize();

            float dashForce = 100.0f;

            b2Vec2 impulse = { dir.x * pbody->body->GetMass() * player->moveForce * dashForce, dir.y * pbody->body->GetMass() * player->moveForce * dashForce};

            pbody->body->ApplyLinearImpulse(impulse, pbody->body->GetWorldCenter(), true);

            player->dashTimer.Start();

            //Animation
            if (player->currentClass == KNIGHT) {
                app->render->DrawTexture(player->SabrinaEspadaDash.texture, player->position.x - 15, player->position.y - 25, &player->SabrinaEspadaDash.GetCurrentFrame());
                player->SabrinaEspadaDash.Update(dt);
            }
            if (player->currentClass == WIZARD) {
                app->render->DrawTexture(player->SabrinaCetroDash.texture, player->position.x - 15, player->position.y - 25, &player->SabrinaCetroDash.GetCurrentFrame());
                player->SabrinaCetroDash.Update(dt);
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