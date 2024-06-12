#include "Core/App.h"
#include "Gameplay/Credits.h"
#include "Core/SceneManager.h"
#include "Core/Window.h"
#include "Core/Textures.h"
#include "Core/Audio.h"
#include "Core/Render.h"
#include "Core/QuestManager.h"

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

	selectBack = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "Back",  { 1262, 818, 200, 50 }, (Module*)this);

	app->render->camera.target = nullptr;
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	// Deactivate quests
	app->questManager->GetQuestById(0)->SetActive(false);
	app->questManager->GetQuestById(1)->SetActive(false);
	app->questManager->GetQuestById(2)->SetActive(false);
	app->questManager->GetQuestById(3)->SetActive(false);
	app->questManager->GetQuestById(4)->SetActive(false);
	app->questManager->GetQuestById(5)->SetActive(false);
	app->questManager->GetQuestById(6)->SetActive(false);
	app->questManager->GetQuestById(7)->SetActive(false);
	app->questManager->GetQuestById(8)->SetActive(false);
	app->questManager->GetQuestById(9)->SetActive(false);
	app->questManager->GetQuestById(10)->SetActive(false);
	app->questManager->GetQuestById(11)->SetActive(false);
	app->questManager->GetQuestById(12)->SetActive(false);

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
	app->tex->UnLoad(credits);
	app->guiManager->RemoveGuiControl(selectBack);
	selectBack = nullptr;

	return true;
}

bool Credits::OnGuiMouseClickEvent(GuiControl *control)
{
	app->audio->PlayFx(FxId);

	switch (control->id) {
	case 1:
		app->sceneManager->ChangeScene("mainmenu");
		break;
	}
    return true;
}