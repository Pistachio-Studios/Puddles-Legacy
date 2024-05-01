#ifndef __APP_H__
#define __APP_H__

#include "Core/Module.h"
#include "Core/ParticleManager.h"
#include "Utils/List.h"
#include "Utils/PerfTimer.h"
#include "Utils/Timer.h"
#include "Core/EntityManager.h"

#include <pugixml.hpp>

// Modules
class Window;
class Input;
class Render;
class Textures;
class Lighting;
class Audio;
class SceneManager;
class EntityManager;
class Map;
class AnimationManager;
class Physics;
class ParticleManager;
class GuiManager;
class DebugUI;
class DialogManager;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;
	const float GetDt() const;
	const float GetSecondsSinceStartup() const;
	const uint64_t GetFrameCount() const;
	const uint32_t GetFramesPerSecond() const;
	const uint32_t GetAverageFps() const;


	// Request a save data in an XML file 
	bool LoadRequest();

	// Request to load data from XML file 
	bool SaveRequest();

private:

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Reads XML file and loads the data
	bool LoadFromFile();

	// Save XML file
	bool SaveFromFile();

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Lighting* lighting;
	Audio* audio;
	SceneManager* sceneManager;
	EntityManager* entityManager;
	Map* map;
	AnimationManager* animationManager;
	Physics* physics;
	ParticleManager* particleManager;
	GuiManager* guiManager;
	DebugUI* debugUI;
	DialogManager* dialogManager;


private:

	int argc;
	char** args;
	SString gameTitle;
	SString organization;

	List<Module *> modules;

	// xml_document to store the config file and
	// xml_node(s) to read specific branches of the xml
	pugi::xml_document configFile;
	pugi::xml_node configNode;

	uint frames;
	float dt;

    // required variables are provided:
	Timer startupTime;
	PerfTimer frameTime;
	PerfTimer lastSecFrameTime;

	uint64_t frameCount = 0;
	uint32_t framesPerSecond = 0;
	uint32_t lastSecFrameCount = 0;

	float averageFps = 0.0f;
	uint32_t secondsSinceStartup = 0;

	float maxFrameDuration = 16.666f;

	float maxFps = 30; // TODO remove legacy code

	bool loadRequest = false;
	bool saveRequest = false;

};

extern App* app;

extern bool debug;
extern bool fpsLimiter;
extern bool freeCam;
extern bool paused;

#endif	// __APP_H__