#ifndef __MINISPIDERATTACKSTATE_H__
#define __MINISPIDERATTACKSTATE_H__

#include "Gameplay/Entities/Enemies/MiniSpider.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"
#include "Gameplay/Entities/Player.h"

class Player;
class MiniSpiderAttackState : public State<MiniSpider> {
private:
    MiniSpider* minispider;
    Player* player;

public:
    MiniSpiderAttackState(SString name) : State(name) {}
    inline void Enter() override
    {
        LOG("MiniSpiderAttackState Enter()");

        minispider = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        LOG("MiniSpiderAttackState Update()");

        //Animation
        app->render->DrawTexture(minispider->spiderAttack.texture, minispider->position.x - 100, minispider->position.y - 150, &minispider->spiderAttack.GetCurrentFrame());
        minispider->spiderAttack.Update(dt);

        /*b2Vec2 attackDirection = { (float32)player->position.x - enemyboss->position.x, (float32)player->position.y - enemyboss->position.y };
        attackDirection.Normalize();

        b2Vec2 attackImpulse = { attackDirection.x, attackDirection.y };

        enemyboss->pbody->body->ApplyLinearImpulse(attackImpulse, enemyboss->pbody->body->GetWorldCenter(), true);*/
        //app->render->DrawRectangle({ minispider->position.x - 5, minispider->position.y - 2, 36, 36 }, 0, 50, 255);

        minispider->attackTimer.Start();
        StateMachineReference->ChangeState("move");
    }
    inline void Exit() override
    {
        LOG("MiniSpiderAttackState Exit()");
    }
};
#endif // __MINISPIDERATTACKSTATE_H__