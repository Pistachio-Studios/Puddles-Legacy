#include "Core/Lighting.h"

#include "Core/App.h"
#include "Core/Textures.h"
#include "Utils/List.h"
#include "Utils/Log.h"
#include <SDL_blendmode.h>
#include <SDL_render.h>
#include <imgui.h>

void Light::Draw()
{
    texture = app->tex->Load("Assets/Textures/light.png");
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_ADD);
    SDL_SetTextureColorMod(texture, color.r, color.g, color.b);
    SDL_SetTextureAlphaMod(texture, color.a);
    app->render->DrawTexture(texture, position.x, position.y, NULL);
}

Lighting::Lighting() : Module()
{
    name.Create("lighting");
}

Lighting::Lighting(bool startEnabled) : Module(startEnabled)
{
    name.Create("lighting");
}

// Destructor
Lighting::~Lighting()
{
}

// Called before Lighting is available
bool Lighting::Awake(pugi::xml_node& conf)
{
    LOG("Loading Lighting");
    bool ret = true;

    return ret;
}

// Called before the first frame
bool Lighting::Start()
{
    //ambientLight = { 60, 50, 70, 255 };
    ambientLight = { 0, 0, 0, 255 };

    AddLight({600,2000}, 0, {255,255,255,255});
    return true;
}

// Called each loop iteration
bool Lighting::PreUpdate()
{
    return true;
}

bool Lighting::Update(float dt)
{
    return true;
}

bool Lighting::PostUpdate()
{
    //Set the ambient light

    // Set the color you want to multiply with
    SDL_SetRenderDrawColor(app->render->renderer, ambientLight.r, ambientLight.g, ambientLight.b, ambientLight.a);

    // Set the blend mode to modulate the color
    SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_MUL); // o SDL_BLENDMODE_MUL, ni idea

    // Fill the entire app->render->renderer with the color
    SDL_RenderFillRect(app->render->renderer, NULL);

    // Reset the blend mode to its default value
    //SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);

    Light* pLight = nullptr;
    for (ListItem<Light*>* item = lights.start; item; item = item->next)
    {
        pLight = item->data;
        
        if (pLight->active)
        {
            pLight->Draw();
        }
    }

    return true;
}

void Lighting::DrawImGui()
{
    for (ListItem<Light*>* item = lights.start; item; item = item->next)
    {
        Light* light = item->data;
        ImGui::DragInt2("Position", &light->position.x, 1.0f);

        ImVec4 imguiColor;
        imguiColor.x = light->color.r / 255.0f;
        imguiColor.y = light->color.g / 255.0f;
        imguiColor.z = light->color.b / 255.0f;
        imguiColor.w = light->color.a / 255.0f;

        ImGui::ColorEdit4("Color", (float*)&imguiColor);

        light->color.r = static_cast<Uint8>(imguiColor.x * 255);
        light->color.g = static_cast<Uint8>(imguiColor.y * 255);
        light->color.b = static_cast<Uint8>(imguiColor.z * 255);
        light->color.a = static_cast<Uint8>(imguiColor.w * 255);
    }
}

// Called before quitting

bool Lighting::CleanUp()
{
    LOG("Freeing Lighting");
    return true;
}

void Lighting::SetAmbientLight(const SDL_Color& color)
{
    ambientLight = color;
}

Light* Lighting::AddLight(iPoint position, int radius, const SDL_Color& color)
{
    Light* light = new Light();
    light->position = position;
    light->color = color;
    light->active = true;
    lights.Add(light);
    return light;
}