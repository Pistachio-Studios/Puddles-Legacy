#include "Core/App.h"
#include "Core/Input.h"
#include "Utils/SString.h"
#include "Core/Render.h"
#include "Utils/Timer.h"
#include "Core/Window.h"
#include "Gameplay/TownScene.h"
#include "Gameplay/Entities/Npcs/Npc.h"
#include "Gameplay/Entities/Npcs/Loco.h"
#include "Core/Map.h"
#include "Core/SceneManager.h"
#include "Utils/Log.h"
#include "Core/GuiControl.h"
#include "Core/GuiManager.h"
  
#include <box2d/b2_body.h>
#include <tracy/Tracy.hpp>

// Destructor
TownScene::~TownScene()
{}

// Called before the first frame
bool TownScene::Enter()
{
	// iterate all objects in the testscene
	// Check https://pugixml.org/docs/quickstart.html#access

	if (parameters.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = parameters.child("player");

		player->Enable();

		// No se porque no va pero deberi de ir ;-;
		if(app->sceneManager->GetPrevScene()->name == "forestscene")
			player->position = { 2900, 260 };
		if(app->sceneManager->GetPrevScene()->name == "tavernscene")
			player->position = { 2900, 2080 };
	}

	if (parameters.child("map")) {
		//Get the map name from the config file and assigns the value in the module
		app->map->name = parameters.child("map").attribute("name").as_string();
		app->map->path = parameters.child("map").attribute("path").as_string();
	}

	if (parameters.child("camera")) {
		//Get the map name from the config file and assigns the value in the module
		app->render->camera.x = parameters.child("camera").attribute("x").as_int();
		app->render->camera.y = parameters.child("camera").attribute("y").as_int();
	}

	if (parameters.child("npcs"))
	{
		pugi::xml_node npcs = parameters.child("npcs");

		for (pugi::xml_node npcsNode = npcs.child("npc"); npcsNode; npcsNode = npcsNode.next_sibling("npc"))
		{
			Npc* npcs = new Npc();
			app->entityManager->AddEntity(npcs);
			npcs->parameters = npcsNode;
			npcs->Start();
		}
	}

	if (parameters.child("loco")) {
		Loco* loco = new Loco();
		app->entityManager->AddEntity(loco);
		loco->parameters = parameters.child("loco");
		loco->Start();
	}

	//app->physics->Enable();
	app->map->Enable();
	app->entityManager->Enable();

	app->render->camera.target = player;
	app->render->camera.useInterpolation = true;
	app->render->camera.lerpSpeed = 4.0f;
	app->render->camera.offset = { 0,0 };

	playingTime = new Timer();

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	//app->tex->GetSize(img, texW, texH);

	//Pause Menu UI
	gcResume = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, "Resume", { (int)windowW / 2 - 175, (int)windowH / 2 - 100, 300, 50 }, this);
	gcResume->SetObserver(this);
	gcResume->state = GuiControlState::DISABLED;

	gcSave = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, "Save", { (int)windowW / 2 - 175, (int)windowH / 2 - 50, 300, 50 }, this);
	gcSave->SetObserver(this);
	gcSave->state = GuiControlState::DISABLED;

	gcSettings = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, "Settings", { (int)windowW / 2 - 175, (int)windowH / 2, 300, 50 }, this);
	gcSettings->SetObserver(this);
	gcSettings->state = GuiControlState::DISABLED;

	gcBackToTitle = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 8, "Back to Title", { (int)windowW / 2 - 175, (int)windowH / 2 + 50, 300, 50 }, this);
	gcBackToTitle->SetObserver(this);
	gcBackToTitle->state = GuiControlState::DISABLED;

	gcExit = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 9, "Exit", { (int)windowW / 2 - 175, (int)windowH / 2 + 100, 300, 50 }, this);
	gcExit->SetObserver(this);
	gcExit->state = GuiControlState::DISABLED;

	PhysBody* changeForest = app->physics->CreateRectangleSensor(1000, 1800, 100, 50, STATIC);
	changeForest->ctype = ColliderType::CHANGESCENE;
	changeForest->listener = player;

	return true;
}

// Called each loop iteration
bool TownScene::PreUpdate()
{
	// OPTICK PROFILIN
	ZoneScoped;

	return true;
}

// Called each loop iteration
bool TownScene::Update(float dt)
{
	// OPTICK PROFILIN
	ZoneScoped;

	if(freeCam)
	{
		float camSpeed = 1;

		if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			app->render->camera.y -= (int)ceil(camSpeed * dt);

		if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			app->render->camera.y += (int)ceil(camSpeed * dt);

		if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			app->render->camera.x -= (int)ceil(camSpeed * dt);

		if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			app->render->camera.x += (int)ceil(camSpeed * dt);
	}
	static Timer* t = new Timer();
	if (t->ReadSec() > 1)
	{
		PhysBody* body = app->physics->CreateCircle(900, 1440, 5, bodyType::DYNAMIC);
		body->body->ApplyLinearImpulseToCenter({ 0.25,0 }, true);
		t->Start();
	}


	//Cambios de escena sin collider
	if (app->entityManager->GetPlayerEntity()->position.x <= 3030 && app->entityManager->GetPlayerEntity()->position.x >= 2870 && app->entityManager->GetPlayerEntity()->position.y <= 1920 && app->entityManager->GetPlayerEntity()->position.y >= 1785){
		app->sceneManager->ChangeScene("tavernscene");
	}

	if (app->entityManager->GetPlayerEntity()->position.x <= 3090 && app->entityManager->GetPlayerEntity()->position.x >= 2755 && app->entityManager->GetPlayerEntity()->position.y <= 90){
		app->sceneManager->ChangeScene("forestscene");
	}


	return true;
}

// Called each loop iteration
bool TownScene::PostUpdate()
{
	// OPTICK PROFILIN
	ZoneScoped;

	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		if(paused)
		{
			paused = false;
			gcResume->state = GuiControlState::NORMAL;
			gcSettings->state = GuiControlState::NORMAL;
			gcBackToTitle->state = GuiControlState::NORMAL;
			gcExit->state = GuiControlState::NORMAL;
			gcSave->state = GuiControlState::NORMAL;
		}
		else
		{
			paused = true;
			gcResume->state = GuiControlState::DISABLED;
			gcSettings->state = GuiControlState::DISABLED;
			gcBackToTitle->state = GuiControlState::DISABLED;
			gcExit->state = GuiControlState::DISABLED;
			gcSave->state = GuiControlState::DISABLED;
		}
	}

	if(exitPressed)
		ret = false;

	return ret;
}

bool TownScene::Exit()
{
	//IMPORTANTE: DESCARGAR EN ORDEN INVERSO AL CARGADO EN EL APP
	app->entityManager->Disable();
	app->map->Disable();

	app->guiManager->RemoveGuiControl(gcScore);
	app->guiManager->RemoveGuiControl(gcLives);
	app->guiManager->RemoveGuiControl(gcResume);
	app->guiManager->RemoveGuiControl(gcSettings);
	app->guiManager->RemoveGuiControl(gcBackToTitle);
	app->guiManager->RemoveGuiControl(gcExit);
	app->guiManager->RemoveGuiControl(gcSave);

	return true;
}

// Called before quitting
bool TownScene::CleanUp()
{
	LOG("Freeing TownScene");

	app->guiManager->RemoveGuiControl(gcScore);
	app->guiManager->RemoveGuiControl(gcLives);
	app->guiManager->RemoveGuiControl(gcResume);
	app->guiManager->RemoveGuiControl(gcSettings);
	app->guiManager->RemoveGuiControl(gcBackToTitle);
	app->guiManager->RemoveGuiControl(gcExit);
	app->guiManager->RemoveGuiControl(gcSave);

	return true;
}

bool TownScene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	switch (control->id)
	{
	case 6:
		paused = true;
		gcResume->state = GuiControlState::DISABLED;
		gcSettings->state = GuiControlState::DISABLED;
		gcBackToTitle->state = GuiControlState::DISABLED;
		gcExit->state = GuiControlState::DISABLED;
		gcSave->state = GuiControlState::DISABLED;
		break;
	case 7:
		break;
	case 8:
		app->sceneManager->ChangeScene("mainmenu");
		break;
	case 9:
		exitPressed = true;
		break;
	case 10:
		app->SaveRequest();
		break;
	}

	return true;
}
