#include "Core/App.h"
#include "Gameplay/TeamLogo.h"
#include "Core/SceneManager.h"
#include "Core/Window.h"
#include "Core/Audio.h"
#include "Gameplay/TeamLogo.h"
#include "Core/Textures.h"
#include "Core/Render.h"

#include "Utils/Log.h"
#include "Core/GuiControl.h"
#include "Core/GuiManager.h"
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

	teamLogo = app->tex->Load(parameters.child("teamlogo").attribute("texturepath").as_string());

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

	//app->render->DrawTexture(teamLogo, 100, 100);

	//if (timer->ReadSec() >= 2)
	//{
	//	LOG("timer teamlogo %d", timer->ReadSec());
	//	app->sceneManager->ChangeScene("teamlogo");
	//}

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
	//app->render.te
	return true;
}
