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
    app->render->DrawTextureLegacy(texture, position.x - 256 * radius / 2, position.y - 256 * radius / 2, NULL, 1.0f, 0.0f, radius, SDL_FLIP_NONE, INT_MAX, INT_MAX, false);
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

    ambientLight = { 255, 255, 255, 255 };
    
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
    // Set the render target to the new texture
    SDL_SetRenderTarget(app->render->renderer, app->render->lightingTarget);

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

    SDL_SetRenderTarget(app->render->renderer, NULL);

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
    for(ListItem<Light*>* item = lights.start; item; item = item->next)
    {
        delete item->data;
    }
    lights.Clear();

    app->tex->UnLoad(lightTexture);

    ambientLight = { 255, 255, 255, 255 };
    return true;
}

void Lighting::SetAmbientLight(const SDL_Color& color)
{
    ambientLight = color;
}

Light* Lighting::AddLight(iPoint position, float radius, const SDL_Color& color)
{
    Light* light = new Light();
    light->position = position;
    light->color = color;
    light->radius = radius;
    light->active = true;
    light->texture = lightTexture;
    lights.Add(light);
    return light;
}
void Lighting::RemoveLight(Light *light)
{
    lights.Del(lights.At(lights.Find(light)));
    delete light;
}

void Lighting::RemoveAllLights()
{
    for(ListItem<Light*>* item = lights.start; item; item = item->next)
    {
        RemoveLight(item->data);
    }
    lights.Clear();
}