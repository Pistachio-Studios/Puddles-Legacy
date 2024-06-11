#include "Core/App.h"
#include "Core/Input.h"
#include "Utils/SString.h"
#include "Core/Render.h"
#include "Utils/Timer.h"
#include "Core/Window.h"
#include "Core/Audio.h"
#include "Core/Textures.h"
#include "Gameplay/TutorialScene.h"
#include "Gameplay/Entities/Enemies/EnemyBoss.h"
#include "Gameplay/Entities/Enemies/Wasp.h"
#include "Gameplay/Entities/Enemies/MiniSpider.h"
#include "Gameplay/Entities/Items/Plant.h"
#include "Gameplay/Entities/Items/ArnicaPlant.h"
#include "Gameplay/Entities/Items/ComfreyPlant.h"
#include "Gameplay/Entities/Items/HepaticaPlant.h"
#include "Gameplay/Entities/Items/WitchHazelPlant.h"
#include "Gameplay/Entities/Items/HawthornPlant.h"
#include "Gameplay/Entities/Npcs/Loco.h"
#include "Gameplay/Entities/Npcs/Npc.h"
#include "Gameplay/Entities/Npcs/Tabernero.h"
#include "Core/Map.h"
#include "Core/Lighting.h"
#include "Core/SceneManager.h"
#include "Utils/Log.h"
#include "Core/GuiControl.h"
#include "Core/GuiManager.h"
#include "Core/QuestManager.h"

#include <box2d/b2_body.h>
#include <tracy/Tracy.hpp>

// Destructor
TutorialScene::~TutorialScene()
{}

// Called before the first frame
bool TutorialScene::Enter()
{
	// iterate all objects in the testscene
	// Check https://pugixml.org/docs/quickstart.html#access

	if (parameters.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = parameters.child("player");
		player->Enable();
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
	app->lighting->Enable();

	app->render->camera.target = player;
	app->render->camera.useInterpolation = true;
	app->render->camera.lerpSpeed = 4.0f;
	app->render->camera.offset = { 0,0 };

	playingTime = new Timer();

	//Get the size of the texture
	//app->tex->GetSize(img, texW, texH);

/// TODO change scene collider

	//PhysBody* changeTown = app->physics->CreateRectangleSensor(1000, 1800, 100, 50, STATIC);
	//changeTown->ctype = ColliderType::CHANGESCENE;
	//changeTown->listener = player;


	if (parameters.child("enemies").child("EnemyBoss")) {
		enemyboss = (EnemyBoss*)app->entityManager->CreateEntity(EntityType::ENEMYBOSS);
		enemyboss->parameters = parameters.child("enemies").child("EnemyBoss");
		enemyboss->Start();
	}

	if (parameters.child("enemies"))
	{
		pugi::xml_node enemies = parameters.child("enemies");

		for (pugi::xml_node MiniSpiderNode = enemies.child("MiniSpider"); MiniSpiderNode; MiniSpiderNode = MiniSpiderNode.next_sibling("MiniSpider"))
		{
			MiniSpider* minispider = (MiniSpider*)app->entityManager->CreateEntity(EntityType::MINISPIDER);
			minispider->parameters = MiniSpiderNode;
			minispider->Start();
		}

		for (pugi::xml_node WaspNode = enemies.child("Wasp"); WaspNode; WaspNode = WaspNode.next_sibling("Wasp"))
		{
			Wasp* wasp = (Wasp*)app->entityManager->CreateEntity(EntityType::WASP);
			wasp->parameters = WaspNode;
			wasp->Start();
		}
	}

	if (parameters.child("Npcs").child("loco")) {
		Loco* loco = new Loco();
		app->entityManager->AddEntity(loco);
		loco->parameters = parameters.child("Npcs").child("loco");
		loco->Start();
	}

	if (parameters.child("Npcs").child("tabernero")) {
		Tabernero* tabernero = new Tabernero();
		app->entityManager->AddEntity(tabernero);
		tabernero->parameters = parameters.child("Npcs").child("tabernero");
		tabernero->Start();
	}

	for (pugi::xml_node potionNode = parameters.child("Potion").first_child(); potionNode; potionNode = potionNode.next_sibling())
	{
		std::string potionType = potionNode.name();
	
		if (potionType == "VitaPotion")
			player->inventory.AddItem("Vita Potion");
		
		if (potionType == "CeleritaPotion") 
			player->inventory.AddItem("Celerita Potion");

		if (potionType == "EtherPotion")
			player->inventory.AddItem("Ether Potion");

		if (potionType == "OblitiusPotion")
			player->inventory.AddItem("Oblitius Potion");
	}

	
	for (pugi::xml_node plantNode = parameters.child("Plants").first_child(); plantNode; plantNode = plantNode.next_sibling())
	{
		std::string plantType = plantNode.name();

		if (plantType == "ArnicaPlant") {
			Plant* plant = new ArnicaPlant("Arnica Plant", 1, "Para craftear la poción de cura o la de aumentar velocidad");
			app->entityManager->AddEntity(plant);
			plant->parameters = plantNode;
			plant->Start();
		}

		if (plantType == "HepaticaPlant") {
			Plant* plant = new HepaticaPlant("Hepatica Plant", 1, "Para craftear la poción de resetear árbol de habilidades");
			app->entityManager->AddEntity(plant);
			plant->parameters = plantNode;
			plant->Start();
		}

		if (plantType == "ComfreyPlant") {
			Plant* plant = new ComfreyPlant("Comfrey Plant", 1, "Para craftear la poción de resetear árbol de habilidades o la de recuperar energia");
			app->entityManager->AddEntity(plant);
			plant->parameters = plantNode;
			plant->Start();
		}

		if (plantType == "WitchHazelPlant") {
			Plant* plant = new WitchHazelPlant("Witch Hazel Plant", 1, "Para craftear la poción de aumentar velocidad");
			app->entityManager->AddEntity(plant);
			plant->parameters = plantNode;
			plant->Start();
		}

		if (plantType == "HawthornPlant") {
			Plant* plant = new HawthornPlant("Hawthorn Plant", 1, "Para craftear la poción de cura o la de recuperar energia");
			app->entityManager->AddEntity(plant);
			plant->parameters = plantNode;
			plant->Start();
		}

	}
	
	//Quests
	Quest* movementQuest = app->questManager->GetQuestById(0);
	movementQuest->SetCompletionAction([=, this]() -> bool {
		static bool W,S,A,D;
		if(app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN and !W)
		{
			W = true;
			movementQuest->AddCompletionValue(100/4);
		}
		if(app->input->GetKey(SDL_SCANCODE_S) == KEY_DOWN and !S)
		{
			S = true;
			movementQuest->AddCompletionValue(100/4);
		}
		if(app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN and !A)
		{
			A = true;
			movementQuest->AddCompletionValue(100/4);
		}
		if(app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN and !D)
		{
			D = true;
			movementQuest->AddCompletionValue(100/4);
		}

		if(W and S and A and D)
		{
			LOG("Quest completed: %s", app->questManager->GetQuestById(0)->GetTitle().GetString());
			return true; // Add a return statement
		}
		return false; // Add a default return statement
	});

	movementQuest->SetActive(true);


	UIFx = app->audio->LoadFx(parameters.child("menu").attribute("FxPath").as_string());


	UIFx = app->audio->LoadFx(parameters.child("menu").attribute("FxPath").as_string());

	return true;
}

// Called each loop iteration
bool TutorialScene::PreUpdate()
{
	// OPTICK PROFILIN
	ZoneScoped;

	return true;
}

// Called each loop iteration
bool TutorialScene::Update(float dt)
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

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) app->LoadRequest();
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) app->SaveRequest();

	return true;
}

// Called each loop iteration
bool TutorialScene::PostUpdate()
{
	// OPTICK PROFILIN
	ZoneScoped;

	bool ret = true;

	return ret;
}

bool TutorialScene::Exit()
{
	//IMPORTANTE: DESCARGAR EN ORDEN INVERSO AL CARGADO EN EL APP
	app->entityManager->Disable();
	app->map->Disable();

	return true;
}

// Called before quitting
bool TutorialScene::CleanUp()
{
	LOG("Freeing testscene");

	return true;
}

bool TutorialScene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);
	app->audio->PlayFx(UIFx);

	return true;
}