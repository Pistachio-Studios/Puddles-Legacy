#include "Core/App.h"
#include "Gameplay/Credits.h"
#include "Core/SceneManager.h"
#include "Core/Window.h"
#include "Core/Textures.h"
#include "Core/Audio.h"
#include "Core/Render.h"

#include "Utils/Log.h"
#include "Core/GuiControl.h"
#include "Core/GuiManager.h"
#include <tracy/Tracy.hpp>


// Destructor
Credits::~Credits()
{}

// Called before the first frame
bool Credits::Enter()
{
	timer = new Timer();

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	credits = app->tex->Load(parameters.attribute("texturepath").as_string());

	FxId = app->audio->LoadFx(parameters.child("credits").attribute("FxPath").as_string());

	return true;
}

// Called each loop iteration
bool Credits::PreUpdate()
{
	// OPTICK PROFILIN
	ZoneScoped;

	return true;
}

// Called each loop iteration
bool Credits::Update(float dt)
{
	// OPTICK PROFILIN
	ZoneScoped;

    app->render->DrawTexture(credits, 0, 0);

	if (selectBack == nullptr){
		selectBack = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Back",  { 1262, 818, 200, 50 }, (Module*)this);
	}
	
	return true;
}

// Called each loop iteration
bool Credits::PostUpdate()
{
	// OPTICK PROFILIN
	ZoneScoped;

	bool ret = true;

	return ret;
}

bool Credits::Exit()
{
	return true;
}

// Called before quitting
bool Credits::CleanUp()
{
	//app->tex->UnLoad(credits);
	//app->guiManager->RemoveGuiControl(selectBack);
	//selectBack = nullptr;

	return true;
}

bool Credits::OnGuiMouseClickEvent(GuiControl *control)
{
	app->audio->PlayFx(FxId);

	switch (control->id) {
	case 1:
		selectBackPressed = true;
		app->tex->UnLoad(credits);
		app->guiManager->RemoveGuiControl(selectBack);
		selectBack = nullptr;
		app->sceneManager->ChangeScene("mainmenu");


		break;
	}
    return true;
}