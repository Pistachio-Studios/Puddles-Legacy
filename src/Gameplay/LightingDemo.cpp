#include "Gameplay/LightingDemo.h"
#include "Core/App.h"
#include "Core/Textures.h"
#include "Core/Render.h"
#include "Core/Lighting.h"
#include <SDL_stdinc.h>
#include <SDL_timer.h>
#include <cstdlib>
#include <vector>

LightingDemo::~LightingDemo()
{
}

bool LightingDemo::Enter()
{
    app->lighting->Enable();
    
    background = app->tex->Load("Assets/Textures/lighting-demo-wall.png");
    app->lighting->SetAmbientLight({ 60, 50, 70, 255 });

    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        int x = rand() % 720;
        int y = rand() % 720;
        int radius = rand() % 100;
        uint8 r = rand() % 256;
        uint8 g = rand() % 256;
        uint8 b = rand() % 256;
        lights[i] = app->lighting->AddLight({ x, y }, radius, { r, g, b, 255 });
    }

    app->render->camera.target = nullptr;
	app->render->camera.x = 0;
	app->render->camera.y = 0;

    return true;
}

bool LightingDemo::PreUpdate()
{
    return true;
}

bool LightingDemo::Update(float dt)
{
    app->render->DrawTexture(background, 0, 0);

    for(int i = 0; i < MAX_LIGHTS; i++)
    {
        int x = static_cast<int>((cos(SDL_GetTicks() * 0.001 + i * MAX_LIGHTS * 2) + 1) / 2 * 1000);
        int y = static_cast<int>((sin(SDL_GetTicks() * 0.001 + i * MAX_LIGHTS * 4) + 1) / 2 * 600);
        lights[i]->position = { x, y };

        uint8 r = sin (SDL_GetTicks() * 0.001 + i * MAX_LIGHTS) * 128 + 128;
        uint8 g = sin (SDL_GetTicks() * 0.002 + i * MAX_LIGHTS) * 128 + 128;
        uint8 b = sin (SDL_GetTicks() * 0.003 + i * MAX_LIGHTS) * 128 + 128;

        lights[i]->color = { r, g, b, 255 };
    }

    //SDL_Color backgroundColor = {static_cast<Uint8>(sin(SDL_GetTicks() * 0.001) * 128 + 128), static_cast<Uint8>(cos(SDL_GetTicks() * 0.001) * 128 + 128), static_cast<Uint8>(sin(SDL_GetTicks() * 0.001) * 128 + 128), 255};

    Uint8 value = static_cast<Uint8>(sin(SDL_GetTicks() * 0.001) * 16 + 16);

    SDL_Color backgroundColor = {value + 20, value, value + 20, 255};
    app->lighting->SetAmbientLight(backgroundColor);

    return true;
}

bool LightingDemo::PostUpdate()
{
    return true;
}

bool LightingDemo::Exit()
{
    return true;
}

bool LightingDemo::CleanUp()
{
    return true;
}