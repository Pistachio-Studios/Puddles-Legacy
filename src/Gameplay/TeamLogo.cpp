#include "Core/App.h"
#include "Gameplay/TeamLogo.h"
#include "Core/SceneManager.h"
#include "Core/Window.h"
#include "Core/Audio.h"
#include "Gameplay/TeamLogo.h"
#include "Core/Textures.h"
#include "Core/Render.h"
#include "Core/VideoPlayer.h"

#include "Utils/Log.h"
#include <tracy/Tracy.hpp>


// Destructor
TeamLogo::~TeamLogo()
{}

// Called before the first frame
bool TeamLogo::Enter()
{
	timer = new Timer();

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//texture = app->tex->Load(parameters.attribute("texturepath").as_string());

	// Load the intro video
	app->videoPlayer->Start();

	return true;
}

// Called each loop iteration
bool TeamLogo::PreUpdate()
{
	// OPTICK PROFILIN
	ZoneScoped;

	return true;
}

// Called each loop iteration
bool TeamLogo::Update(float dt)
{
	// OPTICK PROFILIN
	ZoneScoped;

	/*app->render->DrawTexture(texture, 0, 0);

	if (timer->ReadSec() >= 3)
	{
		LOG("timer teamlogo %d", timer->ReadSec());
		app->sceneManager->ChangeScene("mainmenu");
	}*/

	//Check if video ended revisar
	if (app->videoPlayer->ConvertPixels(0, 1)) {
		app->sceneManager->ChangeScene("mainmenu");
	}

	return true;
}

// Called each loop iteration
bool TeamLogo::PostUpdate()
{
	// OPTICK PROFILIN
	ZoneScoped;

	bool ret = true;

	return ret;
}

bool TeamLogo::Exit()
{
	return true;
}

// Called before quitting
bool TeamLogo::CleanUp()
{
	LOG("Freeing teamlogo");
	//app->tex->UnLoad(texture);

	//cleaning videoPlayer
	if (app->videoPlayer) {
		app->videoPlayer->CleanUp();
	}

	return true;
}
