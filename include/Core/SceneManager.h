#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "Core/App.h"
#include "Module.h"
#include "Utils/DynArray.h"
#include "Utils/List.h"

class Scene;

enum TRANSITION_TYPE
{
    NONE,
    FADE_TO_BLACK,
    SWEEP
};

class SceneManager : public Module
{
public:

    SceneManager();

    SceneManager(bool startEnabled);

    // Destructor
    virtual ~SceneManager();

    // Called before render is available
    bool Awake(pugi::xml_node&);

    // Called before the first frame
    bool Start();

    // Called each loop iteration
    bool PreUpdate();

    // Called each loop iteration
    bool Update(float dt);

    // Called each loop iteration
    bool PostUpdate();

    // Called before quitting
    bool CleanUp();

    void DrawImGui() override;

    Scene* CreateScene(SString sceneName);

    Scene* FindScene(SString sceneName) const;

    void ChangeScene(SString sceneName, float frames = 60, TRANSITION_TYPE type = TRANSITION_TYPE::FADE_TO_BLACK);

    Scene* GetCurrentScene();

    Scene* GetPrevScene();

    // Load / Save
    bool LoadState(pugi::xml_node node);
    bool SaveState(pugi::xml_node node);

public:
private:
    Scene* nextScene = nullptr;
    Scene* currentScene = nullptr;
    Scene* prevScene = nullptr;

    DynArray<Scene*> scenes;

    enum Fade_Step
	{
		NONE,
		TO_BLACK,
		FROM_BLACK
	} currentStep = Fade_Step::NONE;
    
        uint32_t frameCount = 0;
        uint32_t maxFadeFrames = 0;
    
        SDL_Rect screenRect;

    TRANSITION_TYPE transitionType;
};

#endif // !__SCENE_MANAGER_H__