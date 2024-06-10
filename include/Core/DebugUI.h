#ifndef __DEBUG_UI_H__
#define __DEBUG_UI_H__

#include "Core/Module.h"

class DebugUI : public Module
{
public:
    
    // Constructor
    DebugUI();

    DebugUI(bool startEnabled);

    // Destructor
    virtual ~DebugUI();

    // Called before the first frame
    bool Start();

    bool PreUpdate();

    // Called each loop iteration
    bool Update(float dt);

    bool PostUpdate();

    // Called before quitting
    bool CleanUp();

    public:

    bool sceneManagerLoadScene = false;

    bool entityManagerSpawnEntity = false;
    bool entityManagerEntityList = false;
    bool entityManagerInfo = false;

    bool particleManagerGenerators = false;

    bool physicsInfo = false;

    bool renderLighting = false;
    bool renderInfo = false;

    bool PerformanceMetrics = false;

};

#endif // __DEBUG_UI_H__