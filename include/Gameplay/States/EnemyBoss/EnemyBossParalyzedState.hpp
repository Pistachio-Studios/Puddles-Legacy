#ifndef __ENEMYBOSSPARALYZEDSTATE_H__
#define __ENEMYBOSSPARALYZEDSTATE_H__

#include "Gameplay/Entities/Enemies/EnemyBoss.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"
#include "Utils/Timer.h"

class Player;
class EnemyBossParalyzedState : public State<EnemyBoss> {
private:
    EnemyBoss* enemyboss;
    Player* player;

    Timer timer;

public:
    EnemyBossParalyzedState(SString name) : State(name) {}
    inline void Enter() override
    {
        enemyboss = StateMachineReference->owner;
        timer.Start();
    }
    inline void Update(float dt) override
    {
        //Animation
        app->render->DrawTexture(enemyboss->bossIdle.texture, enemyboss->position.x - 60, enemyboss->position.y - 150, &enemyboss->bossIdle.GetCurrentFrame(), 1.0f, enemyboss->pbody->body->GetAngle() * RADTODEG, 1.0f, 1.0f, enemyboss->flip);
        enemyboss->bossIdle.Update(dt);

        enemyboss->paralyzedParticles->emiting = true;

        player = app->entityManager->GetPlayerEntity();

        if (timer.ReadSec() >= 3.0f) {
            StateMachineReference->ChangeState("move");
        }
    }
    inline void Exit() override
    {
    }
};
#endif // __ENEMYBOSSPARALYZEDSTATE_H__