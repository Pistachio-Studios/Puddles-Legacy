#ifndef __PLAYERMOVESTATE_H__
#define __PLAYERMOVESTATE_H__

#include "Core/App.h"
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
        LOG("PlayerMoveState Enter()");

        player = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        LOG("PlayerMoveState Update()");

        GamePad& pad = app->input->pads[0];

        LOG("pad.l_x: %f", pad.l_x);

        PhysBody* pbody = player->pbody;

        b2Vec2 impulse = { 0, 0 };

        if (pbody->body->GetLinearVelocity().Length() < player->maxSpeed)
        {
            if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || pad.l_x < 0.0f) {
                impulse.x = -pbody->body->GetMass() * player->moveForce;
            }
            if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || pad.l_x > 0.0f) {
                impulse.x = pbody->body->GetMass() * player->moveForce;
            }
            if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || pad.l_y < 0.0f) {
               impulse.y = -pbody->body->GetMass() * player->moveForce;
            }
            if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || pad.l_y > 0.0f) {
                impulse.y = pbody->body->GetMass() * player->moveForce;
            }
            pbody->body->ApplyLinearImpulse(impulse, pbody->body->GetWorldCenter(), true);
        }

        if (pbody->body->GetLinearVelocity().Length() < 1.0f)
        {
            StateMachineReference->ChangeState("idle");
        }
    }
    inline void Exit() override
    {
        LOG("PlayerMoveState Exit()");
    }
};
#endif // __PLAYERMOVESTATE_H__