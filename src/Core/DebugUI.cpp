#include "Core/DebugUI.h"
#include "Core/App.h"
#include "Core/Input.h"
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

bool DebugUI::Start()
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

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForSDLRenderer(app->win->window, app->render->renderer);
    ImGui_ImplSDLRenderer2_Init(app->render->renderer);

    //TODO: LOAD FONTS

    return true;
}

bool DebugUI::PreUpdate()
{
    // Start the Dear ImGui frame
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    return true;
}

bool DebugUI::Update(float dt)
{
    if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		debug = !debug;
	}

    if (debug)
    {
        // Main Menu Bar
        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("Entity Manager"))
        {
            if (ImGui::MenuItem("Spawn Entity", NULL, entityManagerSpawnEntity)) {entityManagerSpawnEntity = !entityManagerSpawnEntity;}
            if (ImGui::MenuItem("Entity List", NULL, entityManagerEntityList)) {entityManagerEntityList = !entityManagerEntityList;}
            if (ImGui::MenuItem("Info", NULL, entityManagerInfo)) {entityManagerInfo = !entityManagerInfo;}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Render"))
        {
            if (ImGui::MenuItem("Info", NULL, renderInfo)) {renderInfo = !renderInfo;}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Performance"))
        {
            if (ImGui::MenuItem("Metrics")) {PerformanceMetrics = !PerformanceMetrics;}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();


        // FPS Metrics

        // Modify your PostUpdate function
        if(PerformanceMetrics)
        {
            // Add this at the top of your file or in your class definition
            static float fps_values[1000] = { 0 };
            static int fps_idx = 0;
            ImGui::Begin("Performance Metrics", &PerformanceMetrics, ImGuiWindowFlags_AlwaysAutoResize);

            // Record FPS value
            float current_fps = ImGui::GetIO().Framerate;
            fps_values[fps_idx] = current_fps;
            fps_idx = (fps_idx + 1) % 1000; // Move to the next index, wrap around at 100

            ImGui::Text("FPS: %.1f", current_fps); // Display the current FPS

            // Calculate maximum FPS value
            float max_fps = fps_values[0];
            for (int i = 1; i < 1000; i++) {
                if (fps_values[i] > max_fps) {
                    max_fps = fps_values[i];
                }
            }

            // Change histogram color to red
            //ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            ImGui::PlotHistogram("", fps_values, IM_ARRAYSIZE(fps_values), 0, NULL, 0.0f, max_fps*2, ImVec2(0, 60)); // Plot FPS histogram
            //ImGui::PopStyleColor();

            ImGui::Text("Average FPS: %.1u", app->GetAverageFps());
            ImGui::Text("Frame Duration: %.3f s", app->GetDt() * 0.001f); // Display the frame duration in milliseconds
            ImGui::Text("Time Since Startup: %d s", (int)app->GetSecondsSinceStartup()); // Display the time since the application started
            ImGui::Text("Frame Count: %lu", app->GetFrameCount());

            ImGui::End();
        }
    }

    //ImGui::ShowDemoWindow((bool*)true);

    return true;
}

bool DebugUI::PostUpdate()
{
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