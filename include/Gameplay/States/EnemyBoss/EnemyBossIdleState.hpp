#ifndef __ENEMYBOSSIDLESTATE_H__
#define __ENEMYBOSSIDLESTATE_H__

#include "Gameplay/Entities/Enemies/EnemyBoss.h"
#include "Utils/State.h"
#include "Utils/SString.h"
#include "Utils/Defs.h"

class Player;
class EnemyBossIdleState : public State<EnemyBoss> {
private:
    EnemyBoss* enemyboss;
    Player* player;

public:
    EnemyBossIdleState(SString name) : State(name) {}
    inline void Enter() override
    {
        enemyboss = StateMachineReference->owner;
    }
    inline void Update(float dt) override
    {

        //Animation
        app->render->DrawTexture(enemyboss->bossIdle.texture, enemyboss->position.x - 60, enemyboss->position.y - 150, &enemyboss->bossIdle.GetCurrentFrame(), 1.0f, enemyboss->pbody->body->GetAngle() * RADTODEG, 1.0f, 1.0f, enemyboss->flip);
        enemyboss->bossIdle.Update(dt);


        player = app->entityManager->GetPlayerEntity();

        if (PIXEL_TO_METERS(player->position.DistanceTo(enemyboss->position)) > 3.0f && PIXEL_TO_METERS(player->position.DistanceTo(enemyboss->position)) < 13.0f)
        {
            StateMachineReference->ChangeState("move");
            // AUDIO DONE dog idle
            //app->audio->PlayFx(dogEnemy->dogBark);
        }
    }
    inline void Exit() override
    {
    }
};
#endif // __ENEMYBOSSIDLESTATE_H__