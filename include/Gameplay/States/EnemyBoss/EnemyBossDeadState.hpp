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


        enemyboss = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {

        //Animation
        app->render->DrawTexture(enemyboss->bossDeath.texture, enemyboss->position.x - 60, enemyboss->position.y - 150, &enemyboss->bossDeath.GetCurrentFrame(), 1.0f, enemyboss->pbody->body->GetAngle() * RADTODEG, 1.0f, 1.0f, enemyboss->flip);
        enemyboss->bossDeath.Update(dt);

        if (enemyboss->bossDeath.GetCurrentFrameCount() >= 11) {
            enemyboss->Disable();
            enemyboss->CleanUp();

            for (int i = 0; i < 10; ++i) {
                app->physics->DestroyBody(enemyboss->bulletArray[i]->pbody);
                app->tex->UnLoad(enemyboss->bulletArray[i]->texture);
            }
        }
    }
    inline void Exit() override
    {

    }
};
#endif // __ENEMYBOSSDEADSTATE_H__