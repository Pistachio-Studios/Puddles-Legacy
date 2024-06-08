#include "Core/App.h"
#include "Core/Window.h"
#include "Core/Input.h"
#include "Core/Render.h"
#include "Core/Textures.h"
#include "Core/Audio.h"
#include "Core/SceneManager.h"
#include "Gameplay/Scene.h"
#include "Core/Map.h"
#include "Core/AnimationManager.h"
#include "Core/Physics.h"
#include "Core/ParticleManager.h"
#include "Core/GuiManager.h"
#include "Core/DebugUI.h"
#include "Core/EntityManager.h"
#include "Core/Lighting.h"
#include "Core/VideoPlayer.h"
#include "Core/DialogManager.h"
#include "Core/QuestManager.h"
#include "Core/UI.h"

#include "Utils/Defs.h"
#include "Utils/Log.h"

#include <SDL_image.h>
#include <pugixml.hpp>

#include <tracy/Tracy.hpp>


// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	Timer timer = Timer();
	startupTime = Timer();
	frameTime = PerfTimer();
	lastSecFrameTime = PerfTimer();

	frames = 0;

	win = new Window(true);
	input = new Input(true);
	render = new Render(true);
	tex = new Textures(true);
	lighting = new Lighting(false);
	audio = new Audio(true);
	physics = new Physics(true);
	sceneManager = new SceneManager(true);
	map = new Map(false);
	animationManager = new AnimationManager(true);
	entityManager = new EntityManager(false);
	particleManager = new ParticleManager(true);
	guiManager = new GuiManager(true);
	videoPlayer = new VideoPlayer(true);
	debugUI = new DebugUI(true);
	dialogManager = new DialogManager(true);
	questManager = new QuestManager(true);
	ui = new UI(true);


	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(win);
	AddModule(input);
	AddModule(tex);
	AddModule(audio);
	AddModule(physics);
	AddModule(sceneManager);
	AddModule(map);
	AddModule(animationManager); 
	AddModule(entityManager);
	AddModule(particleManager);
	AddModule(lighting);
	AddModule(guiManager);
	AddModule(videoPlayer);
	AddModule(dialogManager);
	AddModule(questManager);
	AddModule(ui);
	AddModule(debugUI);

	// Render last to swap buffer
	AddModule(render);

	LOG("Timer App Constructor: %f", timer.ReadMSec());
}

// Destructor
App::~App()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.Clear();
}

void App::AddModule(Module* module)
{
	module->Init();
	modules.Add(module);
}

// Called before render is available
bool App::Awake()
{
	srand(time(NULL));

	Timer timer = Timer();

	bool ret = LoadConfig();

	if(ret == true)
	{
		gameTitle = configNode.child("app").child("title").child_value();
		win->SetTitle(gameTitle.GetString());
		maxFrameDuration = configFile.child("config").child("app").child("maxFrameDuration").attribute("value").as_float();

		//set camera size
		render->camera.w = configNode.child("resolution").attribute("width").as_int();
		render->camera.h = configNode.child("resolution").attribute("height").as_int();

		ListItem<Module*>* item;
		item = modules.start;

		while(item != NULL && ret == true)
		{
			// If the section with the module name exists in config.xml, fill the pointer with the valid xml_node
			// that can be used to read all variables for that module.
			// Send nullptr if the node does not exist in config.xml
			LOG("item: %s", item->data->name.GetString());
			pugi::xml_node node = configNode.child(item->data->name.GetString());
			ret = item->data->Awake(node);
			item = item->next;
		}

		//set app icon
		pugi::xml_node icon = configNode.child("app").child("icon");
		SDL_SetWindowIcon(win->window, IMG_Load(icon.attribute("path").as_string()));
	}

	LOG("Timer App Awake(): %f", timer.ReadMSec());

	return ret;
}

// Called before the first frame
bool App::Start()
{
	Timer timer = Timer();

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		if(item->data->active == false) {
			item = item->next;
			continue;
		}

		ret = item->data->Start();
		item = item->next;
	}

	LOG("Timer App Start(): %f", timer.ReadMSec());

	return ret;
}

// Called each loop iteration
bool App::Update()
{

	// OPTICK PROFILIN
	ZoneScoped;

	bool ret = true;
	PrepareUpdate();

	if (input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

// Load config from XML file
bool App::LoadConfig()
{
	bool ret = false;
	pugi::xml_parse_result parseResult = configFile.load_file("config.xml");

	if (parseResult) {
		ret = true;
		configNode = configFile.child("config");
	}
	else {
		LOG("Error in App::LoadConfig(): %s", parseResult.description());
	}

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	// OPTICK PROFILIN
	frameTime.Start();
}

// ---------------------------------------------
void App::FinishUpdate()
{
	// OPTICK PROFILIN
	ZoneScoped;

	// This is a good place to call Load / Save functions
	double currentDt = frameTime.ReadMs();
	if (maxFrameDuration > 0 && currentDt < maxFrameDuration) {
		uint32_t delay = (uint32_t) (maxFrameDuration - currentDt);

		PerfTimer delayTimer = PerfTimer();
		SDL_Delay(delay);
		//LOG("We waited for %I32u ms and got back in %f ms",delay,delayTimer.ReadMs());
	}

	// Amount of frames since startup
	frameCount++;

	// Amount of time since game start (use a low resolution timer)
	secondsSinceStartup = startupTime.ReadSec();
	
	// Amount of ms took the last update (dt)
	if (paused)
		dt = 0;
	else
		dt = (float) frameTime.ReadMs();

	// Amount of frames during the last second
	lastSecFrameCount++;

	// Average FPS for the whole game life
	if (lastSecFrameTime.ReadMs() > 1000) {
		lastSecFrameTime.Start();
		averageFps = (averageFps + lastSecFrameCount) / 2;
		framesPerSecond = lastSecFrameCount; 
		lastSecFrameCount = 0;
	}


	// Shows the time measurements in the window title
	static char title[256];
	#ifdef __linux__
	sprintf_s(title, 256, "%s: Av.FPS: %.2f Last sec frames: %i Last dt: %.3f Time since startup: %u Frame Count: %lu ",
		gameTitle.GetString(), averageFps, framesPerSecond, dt, secondsSinceStartup, frameCount);
	#elif _MSC_VER
	sprintf_s(title, 256, "%s: Av.FPS: %.2f Last sec frames: %i Last dt: %.3f Time since startup: %I32u Frame Count: %I64u ",
		gameTitle.GetString(), averageFps, framesPerSecond, dt, secondsSinceStartup, frameCount);
	#endif

	app->win->SetTitle(title);

	if (loadRequest) {
		loadRequest = false;
		LoadFromFile();
	}

	if (saveRequest) {
		saveRequest = false;
		SaveFromFile();
	}
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	// OPTICK PROFILIN
	ZoneScoped;

	bool ret = true;

	// Activate FPS limiter if Debug mode is ON
	if (fpsLimiter) {
		maxFrameDuration = 1000 * (1 / maxFps);
	} else {
		maxFrameDuration = configFile.child("config").child("app").child("maxFrameDuration").attribute("value").as_float();
	}

	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{

	// OPTICK PROFILIN
	ZoneScoped;

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
		if(debug)item->data->DrawImGui();
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{

	// OPTICK PROFILIN
	ZoneScoped;

	bool ret = true;
	ListItem<Module*>* item;
	Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	Timer timer = Timer();

	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	LOG("Timer App CleanUp(): %f", timer.ReadMSec());

	return ret;
}


/**
 * Loads the game state from a file.
 *
 * This function loads the game state from an XML file specified by the given slot number.
 * It parses the XML file and retrieves the game state data for each module in the application.
 * The game state data is then passed to the corresponding module's LoadState function.
 *
 * @param slot The slot number indicating the save file to load from.
 * @return True if the game state was successfully loaded, false otherwise.
 */
bool App::LoadFromFile() {
    bool ret = true;
	
    pugi::xml_document saveFile;
    std::string filename = "save_game_" + std::to_string(currentSlot) + ".xml";
    pugi::xml_parse_result result = saveFile.load_file(filename.c_str());

    if (result) {
        LOG("%s parsed without errors", filename.c_str());

        ListItem<Module*>* item;
        item = modules.start;

        while (item != NULL && ret == true) {
            ret = item->data->LoadState(saveFile.child("game_state").child(item->data->name.GetString()));
            item = item->next;
        }

    } else {
        LOG("Error loading %s: %s", filename.c_str(), result.description());
        ret = false;
    }

    return ret;
}


/**
 * Saves the game state to a file.
 *
 * @param slot The slot number to save the game to.
 * @return True if the game state was successfully saved, false otherwise.
 */
bool App::SaveFromFile() {
    bool ret = true;

	
    pugi::xml_document saveFile;
    pugi::xml_node gameState = saveFile.append_child("game_state");

    ListItem<Module*>* item;
    item = modules.start;

    while (item != NULL && ret == true)
    {
        pugi::xml_node module = gameState.append_child(item->data->name.GetString());
        ret = item->data->SaveState(module);
        item = item->next;
    }

    std::string filename = "save_game_" + std::to_string(currentSlot) + ".xml";
    ret = saveFile.save_file(filename.c_str());

    if (ret) LOG("Saved to %s", filename.c_str());
    else LOG("Error saving game state");

    return ret;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return gameTitle.GetString();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.GetString();
}

const float App::GetDt() const
{
	return dt;
}

const float App::GetSecondsSinceStartup() const
{
	return secondsSinceStartup;
}

const uint64_t App::GetFrameCount() const
{
	return frameCount;
}

const uint32_t App::GetFramesPerSecond() const
{
	return framesPerSecond;
}

const uint32_t App::GetAverageFps() const
{
	return averageFps;
}


// L14: TODO 1: Implement the methods LoadRequest() and SaveRequest() to request and call the Load / Save the game state at the end of the frame
// The real execution of load / save will be implemented in TODO 5 and 7

// Request a save data in an XML file 
bool App::LoadRequest() {

	bool ret = true;
	loadRequest = true;
	return ret;
}

// Request to load data from XML file 
bool App::SaveRequest() {
	bool ret = true;
	saveRequest = true;
	return true;
}
