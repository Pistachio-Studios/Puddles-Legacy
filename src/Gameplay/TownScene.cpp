#include "Core/App.h"
#include "Core/Input.h"
#include "Core/Render.h"
#include "Core/Window.h"
#include "Core/Map.h"
#include "Core/SceneManager.h"
#include "Core/GuiControl.h"
#include "Core/GuiManager.h"
#include "Gameplay/TownScene.h"
#include "Gameplay/Entities/Npcs/Npc.h"
#include "Gameplay/Entities/Npcs/Loco.h"
#include "Gameplay/Entities/Items/Plant.h"
#include "Utils/Log.h"
#include "Utils/Timer.h"
#include "Utils/SString.h"
#include "Core/QuestManager.h"

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
		if (app->sceneManager->GetPrevScene()->name == "forestscene"){
			comesFromForest = true;
		}

		if (app->sceneManager->GetPrevScene()->name == "tavernscene"){
			comesFromTavern = true;
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

	for (pugi::xml_node plantNode = parameters.child("Plants").first_child(); plantNode; plantNode = plantNode.next_sibling())
	{
		std::string plantType = plantNode.name();

		if (plantType == "ArnicaPlant") {
			Plant* plant = new ArnicaPlant("Arnica Plant", 1, "Permite craftear la poción de cura");
			app->entityManager->AddEntity(plant);
			plant->parameters = plantNode;
			plant->Start();
		}

		if (plantType == "HepaticaPlant") {
			Plant* plant = new HepaticaPlant("Hepatica Plant", 1, "Permite craftear la poción de recuperación de energía");
			app->entityManager->AddEntity(plant);
			plant->parameters = plantNode;
			plant->Start();
		}

		if (plantType == "ComfreyPlant") {
			Plant* plant = new ComfreyPlant("Comfrey Plant", 1, "Permite craftear la poción de resetear árbol de habilidades");
			app->entityManager->AddEntity(plant);
			plant->parameters = plantNode;
			plant->Start();
		}

		if (plantType == "WitchHazelPlant") {
			Plant* plant = new WitchHazelPlant("Witch Hazel Plant", 1, "Permite craftear la poción de resetear árbol de habilidades");
			app->entityManager->AddEntity(plant);
			plant->parameters = plantNode;
			plant->Start();
		}

		if (plantType == "HawthornPlant") {
			Plant* plant = new HawthornPlant("Hawthorn Plant", 1, "Permite craftear la poción de resetear árbol de habilidades");
			app->entityManager->AddEntity(plant);
			plant->parameters = plantNode;
			plant->Start();
		}

	}

	//app->physics->Enable();
	app->map->Enable();
	app->entityManager->Enable();

	app->render->camera.x = -player->position.x + (app->render->camera.w / 2);
	app->render->camera.y = -player->position.y + (app->render->camera.h / 2);
	app->render->camera.target = player;
	app->render->camera.useInterpolation = true;
	app->render->camera.lerpSpeed = 4.0f;
	app->render->camera.offset = { 0,0 };

	playingTime = new Timer();

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	//app->tex->GetSize(img, texW, texH);

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

	// No se porque no va pero deberi de ir ;-;
	if (comesFromForest)
		//player->position = { 2900, 260 };
		player->pbody->body->SetTransform({ PIXEL_TO_METERS(2900),PIXEL_TO_METERS(260) }, 0);
		comesFromForest = false;

	if (comesFromTavern)
		//player->position = { 2900, 2080 };
		player->pbody->body->SetTransform({ PIXEL_TO_METERS(2900),PIXEL_TO_METERS(2080) }, 0);
		comesFromTavern = false;

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

	//Quests
	#pragma region Quests
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
	if (!app->questManager->GetQuestById(0)->IsCompleted())
		movementQuest->SetActive(true);

	Quest* combatQuest = app->questManager->GetQuestById(1);
	combatQuest->SetCompletionAction([=, this]() -> bool {
		static bool click;
		if(app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			click = true;
			combatQuest->AddCompletionValue(100);
		}
		if(click)
		{
			LOG("Quest completed: %s", app->questManager->GetQuestById(1)->GetTitle().GetString());
			return true; // Add a return statement
		}
		return false; // Add a default return statement
	});
	if (!app->questManager->GetQuestById(1)->IsCompleted())
		combatQuest->SetActive(true);

	Quest* dodingQuest = app->questManager->GetQuestById(2);
	dodingQuest->SetCompletionAction([=, this]() -> bool {
		static bool shift;
		if(app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN)
		{
			shift = true;
			dodingQuest->AddCompletionValue(100);
		}
		if(shift)
		{
			LOG("Quest completed: %s", app->questManager->GetQuestById(2)->GetTitle().GetString());
			return true; // Add a return statement
		}
		return false; // Add a default return statement
	});
	if (!app->questManager->GetQuestById(2)->IsCompleted())
		dodingQuest->SetActive(true);

	Quest* bookQuest = app->questManager->GetQuestById(3);
	bookQuest->SetCompletionAction([=, this]() -> bool {
		static bool book;
		if(app->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN)
		{
			book = true;
			bookQuest->AddCompletionValue(100);
		}
		if(book)
		{
			LOG("Quest completed: %s", app->questManager->GetQuestById(3)->GetTitle().GetString());
			return true; // Add a return statement
		}
		return false; // Add a default return statement
	});
	if (!app->questManager->GetQuestById(3)->IsCompleted())
		bookQuest->SetActive(true);

	Quest* potionQuest = app->questManager->GetQuestById(4);
	potionQuest->SetCompletionAction([=, this]() -> bool {
		static bool potion, menu;
		if(app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
		{
			potion = true;
			potionQuest->AddCompletionValue(100 / 2);
		}
		if(app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
		{
			menu = true;
			potionQuest->AddCompletionValue(100 / 2);
		}
		if(potion and menu)
		{
			LOG("Quest completed: %s", app->questManager->GetQuestById(4)->GetTitle().GetString());
			return true; // Add a return statement
		}
		return false; // Add a default return statement
	});
	if (!app->questManager->GetQuestById(4)->IsCompleted())
		potionQuest->SetActive(true);

	Quest* speakBarkeeper = app->questManager->GetQuestById(5);
	speakBarkeeper->SetCompletionAction([=, this]() -> bool {
		static bool speak;
		if(app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			speak = true;
			speakBarkeeper->AddCompletionValue(100);
		}
		if(speak)
		{
			LOG("Quest completed: %s", app->questManager->GetQuestById(5)->GetTitle().GetString());
			return true; // Add a return statement
		}
		return false; // Add a default return statement
	});

	if (
		(app->questManager->GetQuestById(0)->IsCompleted() and 
		app->questManager->GetQuestById(1)->IsCompleted() and 
		app->questManager->GetQuestById(2)->IsCompleted() and 
		app->questManager->GetQuestById(3)->IsCompleted() and 
		app->questManager->GetQuestById(4)->IsCompleted()) or
		!app->questManager->GetQuestById(5)->IsCompleted()) {
			app->questManager->GetQuestById(5)->SetActive(true);
		}
	#pragma endregion


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

	return ret;
}

bool TownScene::Exit()
{
	//IMPORTANTE: DESCARGAR EN ORDEN INVERSO AL CARGADO EN EL APP
	app->entityManager->Disable();
	app->map->Disable();

	return true;
}

// Called before quitting
bool TownScene::CleanUp()
{
	LOG("Freeing TownScene");

	return true;
}

bool TownScene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	return true;
}
