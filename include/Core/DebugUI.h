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

    bool Awake(pugi::xml_node&);

    // Called before the first frame
    bool Start();

    // Called each loop iteration
    bool Update(float dt);

    // Called before quitting
    bool CleanUp();

};

#endif // __DEBUG_UI_H__