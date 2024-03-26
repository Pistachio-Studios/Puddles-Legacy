#include "Core/Lighting.h"

#include "Core/App.h"
#include "Core/DebugUI.h"
#include "Core/Render.h"
#include "Core/Textures.h"
#include "Core/Window.h"
#include "Utils/List.h"
#include "Utils/Log.h"
#include <SDL_blendmode.h>
#include <SDL_render.h>
#include <imgui.h>

void Light::Draw()
{
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
    LOG("Creating Lighting");
    lightTexture = app->tex->Load("Assets/Textures/light.png"); //MEJOR CARGAR ESTO DESDE EL CONFIG!!!

    ambientLight = { 60, 50, 70, 255 };
    //ambientLight = { 0, 0, 0, 255 };

    AddLight({600,2000}, 0, {255,255,255,255});
    
    return true;
}

// Called each loop iteration
bool Lighting::PreUpdate()
{
    return true;
}

bool Lighting::PostUpdate()
{
    return true;
}

bool Lighting::Update(float dt)
{
    // Save the original render target
    SDL_Texture* originalTarget;
    originalTarget = SDL_GetRenderTarget(app->render->renderer);

    // Get the window size
    int windowWidth, windowHeight;
    SDL_GetWindowSize(app->win->window, &windowWidth, &windowHeight);

    // Create a texture with the window size
    SDL_Texture* lightingTarget = SDL_CreateTexture(app->render->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, windowWidth, windowHeight);

    // Set the render target to the new texture
    SDL_SetRenderTarget(app->render->renderer, lightingTarget);

    // Set the color you want to multiply with
    SDL_SetRenderDrawColor(app->render->renderer, ambientLight.r, ambientLight.g, ambientLight.b, ambientLight.a);

    // Set the blend mode to modulate the color
    //SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_MUL); // o SDL_BLENDMODE_MUL, ni idea

    // Fill the entire app->render->renderer with the color
    SDL_RenderFillRect(app->render->renderer, NULL);

    Light* pLight = nullptr;
    for (ListItem<Light*>* item = lights.start; item; item = item->next)
    {
        pLight = item->data;
        
        if (pLight->active)
        {
            pLight->Draw();
        }
    }

    SDL_SetRenderTarget(app->render->renderer, originalTarget);

    // Set the blend mode to multiply the colors
    SDL_SetTextureBlendMode(lightingTarget, SDL_BLENDMODE_MUL); //o SDL_BLENDMODE_MOD

    // Draw the lightingTarget
    SDL_RenderCopy(app->render->renderer, lightingTarget, NULL, NULL);

    SDL_DestroyTexture(lightingTarget);
    //SDL_DestroyTexture(originalTarget);

    // Reset the blend mode to its default value
    SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);

    SDL_DestroyTexture(originalTarget);

    return true;
}

void Lighting::DrawImGui()
{
    if(app->debugUI->renderLighting)
    {
        ImGui::Begin("Lighting", &app->debugUI->renderLighting, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Ambient Light:");
        
        ImVec4 imguiColor;
        imguiColor.x = ambientLight.r / 255.0f;
        imguiColor.y = ambientLight.g / 255.0f;
        imguiColor.z = ambientLight.b / 255.0f;
        imguiColor.w = ambientLight.a / 255.0f;

        ImGui::ColorEdit4("Color", (float*)&imguiColor);

        ambientLight.r = static_cast<Uint8>(imguiColor.x * 255);
        ambientLight.g = static_cast<Uint8>(imguiColor.y * 255);
        ambientLight.b = static_cast<Uint8>(imguiColor.z * 255);
        ambientLight.a = static_cast<Uint8>(imguiColor.w * 255);

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
        ImGui::End();
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
    light->texture = lightTexture;
    lights.Add(light);
    return light;
}