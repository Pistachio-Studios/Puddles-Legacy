#include "Core/SceneManager.h"
#include "Core/App.h"
#include "Core/DebugUI.h"
#include "Core/Render.h"
#include "Gameplay/LightingDemo.h"
#include "Gameplay/MainMenu.h"
#include "Gameplay/TeamLogo.h"
#include "Gameplay/Credits.h"
#include "Gameplay/ParticleDemo.h"
#include "Gameplay/Scene.h"
#include "Gameplay/TestScene.h"
#include "Gameplay/Level1Scene.h"
#include "Gameplay/ForestScene.h"
#include "Gameplay/TownScene.h"
#include "Gameplay/TutorialScene.h"
#include "Gameplay/TavernScene.h"
#include "Gameplay/Intro.h"
#include "Utils/Defs.h"
#include "Utils/Log.h"
#include <cassert>
#include <imgui.h>
#include <vector>


SceneManager::SceneManager() : Module()
{
    name.Create("scene_manager");
}

SceneManager::SceneManager(bool startEnabled) : Module(startEnabled)
{
    name.Create("scene_manager");
}

// Destructor
SceneManager::~SceneManager()
{
}

// Called before render is available
bool SceneManager::Awake(pugi::xml_node& config)
{
    LOG("Loading Scene Manager");

    bool ret = true;

    // iterate all children of scenemanager node and load all scenes taking the name of the scene
    for (pugi::xml_node sceneNode : config.children())
    {
        // Get the name of the scene
        std::string sceneName = sceneNode.name();

        // Create a new scene with the name
        Scene* scene = CreateScene(sceneName.c_str());
        if(scene == nullptr)
        {
            //ret = false;
            break;
        }

        scene->parameters = sceneNode;

        if(sceneName == config.attribute("startScene").as_string()) //no se si es lo mejor hacerlo asi
        {
            currentScene = scene;
        }

        // Add the scene to the list of scenes
        scenes.PushBack(scene);
    }
    
    if(currentScene == nullptr)
    {
        LOG("No start scene found");
        ret = false;
    }

    return ret;
}

// Called before the first frame
bool SceneManager::Start()
{
    bool ret = true;

    ret = currentScene->Enter();

    return ret;
}

// Called each loop iteration
bool SceneManager::PreUpdate()
{
    bool ret = true;

    ret = currentScene->PreUpdate();

    return ret;
}

// Called each loop iteration
bool SceneManager::Update(float dt)
{
    bool ret = true;

    ret = currentScene->Update(dt);

    if(currentStep == Fade_Step::NONE) return true;

    screenRect = {0, 0, app->render->camera.w, app->render->camera.h};

    if(currentStep == Fade_Step::TO_BLACK)
    {
        ++frameCount;
        if(frameCount >= maxFadeFrames)
        {
            currentScene->Exit();

            if (app->physics->DestroyAllWorldBodies()) {
            //app->lighting->RemoveAllLights();
            app->lighting->SetAmbientLight({255, 255, 255, 255});

            prevScene = currentScene;
            currentScene = nextScene;
            };

            currentScene->Enter();

            currentStep = Fade_Step::FROM_BLACK;
        }
    }
    else
    {
        --frameCount;
        if(frameCount <= 0)
        {
            currentStep = Fade_Step::NONE;
        }
    }

    return ret;
}

bool SceneManager::PostUpdate()
{
    bool ret = true;

    ret = currentScene->PostUpdate();

    // Exit this function if we are not performing a fade
	if (currentStep == Fade_Step::NONE) return true;

    float fadeRatio = (float)frameCount / (float)maxFadeFrames;
    
    switch (transitionType)
    {
        case TRANSITION_TYPE::NONE:
        break;
        case TRANSITION_TYPE::FADE_TO_BLACK:
        {
            // Render the black square with alpha on the screen
            SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(fadeRatio * 255.0f));
            SDL_RenderFillRect(app->render->renderer, &screenRect);
        }
        break;
        case TRANSITION_TYPE::SWEEP:
        {
            SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, 255);
            SDL_Rect rect = {0, 0, app->render->camera.w * fadeRatio, app->render->camera.h};
            SDL_RenderFillRect(app->render->renderer, &rect);
        }
    }

    return ret;
}

// Called before quitting
bool SceneManager::CleanUp()
{
    bool ret = true;

    for(int i = 0; i < scenes.Count(); i++)
    {
        scenes[i]->CleanUp();
        RELEASE(scenes[i]);
    }

    scenes.Clear();

    return ret;
}

void SceneManager::DrawImGui()
{
    if(app->debugUI->sceneManagerLoadScene){
        ImGui::Begin("Load Scene", &app->debugUI->sceneManagerLoadScene, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("Scene List:");

        // Create an array of scene names
        std::vector<const char*> sceneNames;
        for (int i = 0; i < scenes.Count(); i++)
        {
            sceneNames.push_back(scenes[i]->name.GetString());
        }

        // Create a static variable to store the current selection
        static int currentSelection = 0;

        // Create the list box
        ImGui::ListBox("##Scenes", &currentSelection, &sceneNames[0], sceneNames.size());

        // If the selected scene is not the current scene, change the scene
        if (ImGui::Button("Load Scene") && scenes[currentSelection]->name != currentScene->name)
        {
            ChangeScene(scenes[currentSelection]->name);
        }

        ImGui::End();
    /* 
        ImGui::Begin("Scene Manager");
        ImGui::Text("Scene List:");

        // Create an array of scene names
        std::vector<const char*> sceneNames;
        for (int i = 0; i < scenes.Count(); i++)
        {
            sceneNames.push_back(scenes[i]->name.GetString());
        }

        // Create a static variable to store the current selection
        static int currentSelection = 0;

        // Create the list box
        if (ImGui::ListBox("##Scenes", &currentSelection, &sceneNames[0], sceneNames.size()))
        {
            // If the selected scene is not the current scene, change the scene
            if (scenes[currentSelection]->name != currentScene->name)
            {
                ChangeScene(scenes[currentSelection]->name);
            }
        }

        ImGui::End(); */
    }
}

Scene* SceneManager::CreateScene(SString sceneName)
{
    if(sceneName == "mainmenu")
    {
        return new MainMenu(sceneName);
    }
    else if (sceneName == "teamlogo")
    {
        return new TeamLogo(sceneName);
    }
    else if (sceneName == "credits")
    {
        return new Credits(sceneName);
    }
    else if(sceneName == "testscene")
    {
        return new TestScene(sceneName);
    }
    else if(sceneName == "level1scene")
    {
        return new Level1Scene(sceneName);
    }
    else if (sceneName == "forestscene")
    {
        return new ForestScene(sceneName);
    }
    else if(sceneName == "townscene")
    {
        return new TownScene(sceneName);
    }
    else if(sceneName == "tavernscene")
    {
        return new TavernScene(sceneName);
    }
    else if(sceneName == "tutorialscene")
    {
        return new TutorialScene(sceneName);
    }
    else if(sceneName == "particle_demo")
    {
        return new ParticleDemo(sceneName);
    }
    else if(sceneName == "lighting_demo")
    {
        return new LightingDemo(sceneName);
    }
    else if (sceneName == "intro")
    {
        return new Intro(sceneName);
    }
    else
    {
        LOG_ERROR("Scene %s not found\n", sceneName.GetString());
        return nullptr;
    }
}
Scene* SceneManager::GetCurrentScene()
{
    return currentScene;
}

Scene* SceneManager::GetPrevScene()
{
    return prevScene;
}

Scene* SceneManager::FindScene(SString sceneName) const
{
    Scene* scene = nullptr;

    for(int i = 0; i < scenes.Count(); i++)
    {
        if(scenes[i]->name == sceneName)
        {
            scene = scenes[i];
            break;
        }
    }

    return scene;
}

void SceneManager::ChangeScene(SString sceneName, float frames, TRANSITION_TYPE transition)
{

    Scene* newScene = FindScene(sceneName);
    if (newScene != nullptr){

        if(currentStep == Fade_Step::NONE)
        {
            currentStep = Fade_Step::TO_BLACK;
            frameCount = 0;
            maxFadeFrames = frames;
            transitionType = transition;
            nextScene = newScene;
        }
    }
    else{
        LOG("Scene %s not found\n", sceneName.GetString());
    }
}

// Load / Save
bool SceneManager::LoadState(pugi::xml_node node)
{
    bool ret = true;

    ret = currentScene->LoadState(node);

    return ret;
}

bool SceneManager::SaveState(pugi::xml_node node)
{
    bool ret = true;

    // Save the current scene name
    pugi::xml_node sceneNode = node.append_child("currentScene");

    // Check if the current scene is "mainmenu"
    if (strcmp(currentScene->name.GetString(), "mainmenu") == 0) {
        // If it is, save "tutorialscene" instead
        sceneNode.append_attribute("name").set_value("tutorialscene");
        LOG("Saving current scene: tutorialscene");
    } else {
        // If it's not, save the actual current scene name
        sceneNode.append_attribute("name").set_value(currentScene->name.GetString());
        LOG("Saving current scene: %s", currentScene->name.GetString());
    }

    ret = currentScene->SaveState(node);

    return ret;
}