#include "Core/DebugUI.h"
#include "Core/App.h"
#include "Core/Window.h"
#include "Core/Render.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include <SDL2/SDL.h>

DebugUI::DebugUI() :Module()
{
    name.Create("debugUI");
}

DebugUI::DebugUI(bool startEnabled) : Module(startEnabled)
{
    name.Create("debugUI");
}

DebugUI::~DebugUI() {}

bool DebugUI::Awake(pugi::xml_node&)
{
    // From 2.0.18: Enable native IME.
    #ifdef SDL_HINT_IME_SHOW_UI
        SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
    #endif

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    /* // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForSDLRenderer(app->win->window, app->render->renderer);
    ImGui_ImplSDLRenderer2_Init(app->render->renderer); */

    //TODO: LOAD FONTS

    return true;
}

bool DebugUI::Start()
{
    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForSDLRenderer(app->win->window, app->render->renderer);
    ImGui_ImplSDLRenderer2_Init(app->render->renderer);

    return true;
}

bool DebugUI::Update(float dt)
{
    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow((bool*)true);

    // Rendering
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

    return true;
}

bool DebugUI::CleanUp()
{
    // Cleanup
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    return true;
}