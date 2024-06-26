#include "Core/App.h"
#include "Core/Input.h"
#include "Utils/SString.h"
#include "Core/Render.h"
#include "Utils/Timer.h"
#include "Core/Window.h"
#include "Gameplay/TestScene.h"
#include "Core/Map.h"
#include "Core/AnimationManager.h"
#include "Core/SceneManager.h"
#include "Utils/Log.h"
#include "Core/GuiControl.h"
#include "Core/GuiManager.h"
#include "Gameplay/Entities/Npcs/Loco.h"
#include "Gameplay/Entities/Npcs/Tabernero.h"

#include <box2d/b2_body.h>
#include <tracy/Tracy.hpp>

// Destructor
TestScene::~TestScene()
{}

// Called before the first frame
bool TestScene::Enter()
{
	// iterate all objects in the testscene
	// Check https://pugixml.org/docs/quickstart.html#access
	
	if (parameters.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = parameters.child("player");
		player->Enable();
	}

	if (parameters.child("enemies").child("EnemyBoss")) {
		enemyboss = (EnemyBoss*)app->entityManager->CreateEntity(EntityType::ENEMYBOSS);
		enemyboss->parameters = parameters.child("enemies").child("EnemyBoss");
		enemyboss->Enable();
	}

	if (parameters.child("enemies"))
	{
		pugi::xml_node enemies = parameters.child("enemies");

		for (pugi::xml_node MiniSpiderNode = enemies.child("MiniSpider"); MiniSpiderNode; MiniSpiderNode = MiniSpiderNode.next_sibling("MiniSpider"))
		{
			MiniSpider* minispider = (MiniSpider*)app->entityManager->CreateEntity(EntityType::MINISPIDER);
			minispider->parameters = MiniSpiderNode;
		}

		for (pugi::xml_node WaspNode = enemies.child("Wasp"); WaspNode; WaspNode = WaspNode.next_sibling("Wasp"))
		{
			Wasp* wasp = (Wasp*)app->entityManager->CreateEntity(EntityType::WASP);
			wasp->parameters = WaspNode;
		}
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

	return true;
}

// Called each loop iteration
bool TestScene::PreUpdate()
{
	// OPTICK PROFILIN
	ZoneScoped;

	return true;
}

// Called each loop iteration
bool TestScene::Update(float dt)
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

	return true;
}

// Called each loop iteration
bool TestScene::PostUpdate()
{
	// OPTICK PROFILIN
	ZoneScoped;

	bool ret = true;

	return ret;
}

bool TestScene::Exit()
{
	//IMPORTANTE: DESCARGAR EN ORDEN INVERSO AL CARGADO EN EL APP
	app->entityManager->Disable();
	app->map->Disable();

	return true;
}

// Called before quitting
bool TestScene::CleanUp()
{
	LOG("Freeing testscene");

	return true;
}

bool TestScene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	return true;
}
