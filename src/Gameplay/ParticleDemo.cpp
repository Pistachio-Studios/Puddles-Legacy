#include "Core/App.h"
#include "Gameplay/ParticleDemo.h"
#include "Core/Animation.h"
#include "Core/ParticleManager.h"
#include "Core/Render.h"
#include "Core/Textures.h"


#include <SDL_blendmode.h>
#include <SDL_render.h>
#include <cstddef>

ParticleDemo::~ParticleDemo()
{
}

bool ParticleDemo::Enter()
{
    particleSource1 = new ParticleGenerator();
    particleSource1->position = { 473, 319 };
    particleSource1->amount = 30;
    particleSource1->spawnRadius = 1;
    particleSource1->lifetime = 3.0f;
    particleSource1->direction = { 0, -1 };
    particleSource1->spread = 18;
    particleSource1->opacityFade = -1.0f;
    particleSource1->color = { 230, 98, 18, 255 };


    Animation smoke = Animation();
    smoke.texture = app->tex->Load("Assets/Textures/Particles/smoke_scaled.png");
    SDL_SetTextureBlendMode(smoke.texture, SDL_BLENDMODE_ADD);
    smoke.PushBack({0,0,114,114});
    smoke.PushBack({114,0,114,114});
    smoke.PushBack({228,0,114,114});
    smoke.PushBack({342,0,114,114});
    smoke.PushBack({456,0,114,114});
    smoke.PushBack({570,0,114,114});
    smoke.PushBack({0,114,114,114});
    smoke.PushBack({114,114,114,114});
    smoke.PushBack({228,114,114,114});
    smoke.PushBack({342,114,114,114});
    smoke.PushBack({456,114,114,114});
    smoke.PushBack({570,114,114,114});
    smoke.PushBack({0,228,114,114});
    smoke.PushBack({114,228,114,114});
    smoke.PushBack({228,228,114,114});
    smoke.PushBack({342,228,114,114});
    smoke.PushBack({456,228,114,114});
    smoke.PushBack({570,228,114,114});
    smoke.PushBack({0,342,114,114});
    smoke.PushBack({114,342,114,114});
    smoke.PushBack({228,342,114,114});
    smoke.PushBack({342,342,114,114});
    smoke.PushBack({456,342,114,114});
    smoke.PushBack({570,342,114,114});

    smoke.speed = 16.0f;
    smoke.loop = true;


    particleSource1->anim = smoke;

    return true;
}

bool ParticleDemo::PreUpdate()
{
    return true;
}

bool ParticleDemo::Update(float dt)
{
    particleSource1->position = { app->input->GetMouseX(), app->input->GetMouseY() };
    return true;
}

bool ParticleDemo::PostUpdate()
{
    return true;
}

bool ParticleDemo::Exit()
{
    return true;
}

bool ParticleDemo::CleanUp()
{
    return true;
}

