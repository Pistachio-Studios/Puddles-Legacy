#ifndef __ENEMYBOSSDEADSTATE_H__
#define __ENEMYBOSSDEADSTATE_H__

#include "Gameplay/Entities/Enemies/EnemyBoss.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"

class EnemyBossDeadState : public State<EnemyBoss> {
private:
    EnemyBoss* enemyboss;

public:
    EnemyBossDeadState(SString name) : State(name) {}
    inline void Enter() override
    {
        LOG("EnemyBossHurtState Enter()");

        enemyboss = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {
        LOG("EnemyBossDeadState Update()");

        //Animation
        app->render->DrawTexture(enemyboss->bossDeath.texture, enemyboss->position.x - 120, enemyboss->position.y - 230, &enemyboss->bossDeath.GetCurrentFrame(), 1.0f, enemyboss->pbody->body->GetAngle() * RADTODEG, enemyboss->flip);
        enemyboss->bossDeath.Update(dt);

        if (enemyboss->bossDeath.GetCurrentFrameCount() >= 11) {
            app->physics->DestroyBody(enemyboss->pbody);
            enemyboss->Disable();
        }
    }
    inline void Exit() override
    {
        LOG("EnemyBossDeadState Exit()");
    }
};
#endif // __ENEMYBOSSDEADSTATE_H__