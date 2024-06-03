#include "Core/App.h"
#include "Core/SceneManager.h"
#include "Core/Window.h"
#include "Core/Audio.h"
#include "Gameplay/Intro.h"
#include "Core/VideoPlayer.h"

#include "Utils/Log.h"
#include "Core/GuiControl.h"
#include "Core/GuiManager.h"
#include <tracy/Tracy.hpp>


// Destructor
Intro::~Intro()
{}

// Called before the first frame
bool Intro::Enter()
{
	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	// Load the intro video
	app->videoPlayer->ChangeVideo("Assets/Video/Intro/example.mp4");

	return true;
}

// Called each loop iteration
bool Intro::PreUpdate()
{
	// OPTICK PROFILIN
	ZoneScoped;

	return true;
}

// Called each loop iteration
bool Intro::Update(float dt)
{
	// OPTICK PROFILIN
	ZoneScoped;

	return true;
}

// Called each loop iteration
bool Intro::PostUpdate()
{
	// OPTICK PROFILIN
	ZoneScoped;

	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	if(exitPressed)
		ret = false;

	return ret;
}

bool Intro::Exit()
{
	return true;
}

// Called before quitting
bool Intro::CleanUp()
{
	LOG("Freeing Intro");

	return true;
}

