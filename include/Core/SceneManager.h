#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__

#include "Core/App.h"
#include "Module.h"
#include "Utils/DynArray.h"
#include "Utils/List.h"

class Scene;

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

    void ChangeScene(SString sceneName);

    Scene* GetCurrentScene();

    // Load / Save
    bool LoadState(pugi::xml_node&);
    bool SaveState(pugi::xml_node&) const;

public:
    Scene* currentScene = nullptr;
private:
    DynArray<Scene*> scenes;
};

#endif // !__SCENE_MANAGER_H__