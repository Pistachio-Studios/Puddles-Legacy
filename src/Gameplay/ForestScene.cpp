#include "Core/App.h"
#include "Core/Input.h"
#include "Core/Render.h"
#include "Core/Audio.h"
#include "Core/Textures.h"
#include "Core/Window.h"
#include "Core/GuiControl.h"
#include "Core/GuiManager.h"
#include "Core/Map.h"
#include "Core/SceneManager.h"
#include "Gameplay/ForestScene.h"
#include "Gameplay/Entities/Npcs/Loco.h"
#include "Gameplay/Entities/Npcs/Npc.h"
#include "Gameplay/Entities/Npcs/Tabernero.h"
#include "Gameplay/Entities/Items/Button.h"
#include "Gameplay/Entities/Items/Ball.h"
#include "Gameplay/Entities/Items/Plant.h"
#include "Gameplay/Entities/Enemies/EnemyBoss.h"
#include "Gameplay/Entities/Enemies/Wasp.h"
#include "Gameplay/Entities/Enemies/MiniSpider.h"
#include "Utils/SString.h"
#include "Utils/Timer.h"
#include "Utils/Log.h"
#include "Core/QuestManager.h"


#include <box2d/b2_body.h>
#include <tracy/Tracy.hpp>

// Destructor
ForestScene::~ForestScene()
{}

// Called before the first frame
bool ForestScene::Enter()
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

	//Enemies
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
			minispider->Enable();
		}

		for (pugi::xml_node WaspNode = enemies.child("Wasp"); WaspNode; WaspNode = WaspNode.next_sibling("Wasp"))
		{
			Wasp* wasp = (Wasp*)app->entityManager->CreateEntity(EntityType::WASP);
			wasp->parameters = WaspNode;
			wasp->Enable();
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

	// Items
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

	pugi::xml_node assetsNode = parameters.child("assets");

	pugi::xml_node buttonNode = assetsNode.child("button");

	for (int i = 0; buttonNode.attribute("type").as_int() == 1; ++i) {
		buttonList[i] = new Button();
		app->entityManager->AddEntity(buttonList[i]);
		buttonList[i]->parameters = buttonNode;
		buttonList[i]->Enable();
		buttonNode = buttonNode.next_sibling("button");
	}

	for (int i = 0; buttonNode.attribute("type").as_int() == 2; ++i) {
		buttonColourList[i] = new Button();
		app->entityManager->AddEntity(buttonColourList[i]);
		buttonColourList[i]->parameters = buttonNode;
		buttonColourList[i]->Enable();
		buttonNode = buttonNode.next_sibling("button");
	}

	for (int i = 0; buttonNode.attribute("type").as_int() == 3; ++i) {
		buttonBallList[i] = new Button();
		app->entityManager->AddEntity(buttonBallList[i]);
		buttonBallList[i]->parameters = buttonNode;
		buttonBallList[i]->Enable();
		buttonNode = buttonNode.next_sibling("button");
	}

	pugi::xml_node ballNode = assetsNode.child("ball");
	
	for (int i = 0; ballNode; ++i){
		ballList[i] = new Ball();
		app->entityManager->AddEntity(ballList[i]);
		ballList[i]->parameters = ballNode;
		ballList[i]->Enable();
		ballNode = ballNode.next_sibling("ball");
	}

	for (pugi::xml_node plantNode = parameters.child("Plants").first_child(); plantNode; plantNode = plantNode.next_sibling())
	{
		std::string plantType = plantNode.name();

		if (plantType == "ArnicaPlant") {
			Plant* plant = new ArnicaPlant("Arnica Plant", 1, "Para craftear la poción de cura o la de aumentar velocidad");			app->entityManager->AddEntity(plant);
			plant->parameters = plantNode;
			plant->Start();
		}

		if (plantType == "HepaticaPlant") {
			Plant* plant = new HepaticaPlant("Hepatica Plant", 1, "Para craftear la poción de resetear árbol de habilidades");			app->entityManager->AddEntity(plant);
			plant->parameters = plantNode;
			plant->Start();
		}

		if (plantType == "ComfreyPlant") {
			Plant* plant = new ComfreyPlant("Comfrey Plant", 1, "Para craftear la poción de resetear árbol de habilidades o la de recuperar energia");			app->entityManager->AddEntity(plant);
			plant->parameters = plantNode;
			plant->Start();
		}

		if (plantType == "WitchHazelPlant") {
			Plant* plant = new WitchHazelPlant("Witch Hazel Plant", 1, "Para craftear la poción de aumentar velocidad");			app->entityManager->AddEntity(plant);
			plant->parameters = plantNode;
			plant->Start();
		}

		if (plantType == "HawthornPlant") {
			Plant* plant = new HawthornPlant("Hawthorn Plant", 1, "Para craftear la poción de cura o la de recuperar energia");			app->entityManager->AddEntity(plant);
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

	loseScreenTex = app->tex->Load("Assets/Textures/loseScreen.png"); 
	winScreenTex = app->tex->Load("Assets/Textures/winScreen.png"); 

	bush = app->tex->Load("Assets/Maps/Forest-Scene/bush.PNG");

	puzzleFx = app->audio->LoadFx(parameters.child("map").attribute("puzzleFxPath").as_string());
	Music =app->audio->PlayMusic(parameters.child("map").attribute("ForestMusic").as_string());

	door1Closed = false;
	door3Closed = false;
	doorBossClosed = false;
	doorBoss2Closed = false;
	puzzle1 = false;
	puzzle2 = false;
	puzzle3 = false;

	player->bestiary->forestUnlocked = true;



	winTrigger = app->physics->CreateRectangle(33630, 11524, 600, 200, bodyType::STATIC);
	winTrigger->ctype = ColliderType::WINCONDITION;

	return true;
}

void ForestScene::generateRandomPotion() {
	int randomValue = rand() % 3;
	SString potionName = "";
	switch (randomValue)
	{
	case 0:
		potionName = "Vita Potion";
		break;
	case 1:
		potionName = "Celerita Potion";
		break;
	case 2:
		potionName = "Ether Potion";
		break;
	case 3:
		potionName = "Oblitius Potion";
		break;
	}

	if (player->inventory.HasItem(potionName.GetString()))
	{
		player->inventory.GetItem(potionName.GetString())->quantity++;
	}
}

// Called each loop iteration
bool ForestScene::PreUpdate()
{
	// OPTICK PROFILIN
	ZoneScoped;

	return true;
}

// Called each loop iteration
bool ForestScene::Update(float dt)
{
	// OPTICK PROFILIN
	ZoneScoped;

	if (player->bestiary->forestUnlocked) {
		app->questManager->GetQuestById(7)->SetCompleted(true);
	}

	#pragma region Quests
	Quest* exploreForestQuest = app->questManager->GetQuestById(10);
	exploreForestQuest->SetCompletionAction([=, this]() -> bool {
		static bool explore;
		int totalObjectives = 8; // total number of objectives
		int completedObjectives = 0; // completed objectives

		static bool enemy1KilledFlag = false;
		static bool enemy2KilledFlag = false;
		static bool enemy3KilledFlag = false;
		static bool arnicaPlantCollectedFlag = false;
		static bool comfreyPlantCollectedFlag = false;
		static bool hawthornPlantCollectedFlag = false;
		static bool witchhazelPlantCollectedFlag = false;
		static bool hepaticaPlantCollectedFlag = false;
		
		if(player->bestiary->enemy1Killed && !enemy1KilledFlag) {
			completedObjectives++;
			enemy1KilledFlag = true;
		}
		if(player->bestiary->enemy2Killed && !enemy2KilledFlag) {
			completedObjectives++;
			enemy2KilledFlag = true;
		}
		if(player->bestiary->enemy3Killed && !enemy3KilledFlag) {
			completedObjectives++;
			enemy3KilledFlag = true;
		}
		if(player->bestiary->arnicaPlantCollected && !arnicaPlantCollectedFlag) {
			completedObjectives++;
			arnicaPlantCollectedFlag = true;
		}
		if(player->bestiary->comfreyPlantCollected && !comfreyPlantCollectedFlag) {
			completedObjectives++;
			comfreyPlantCollectedFlag = true;
		}
		if(player->bestiary->hawthornPlantCollected && !hawthornPlantCollectedFlag) {
			completedObjectives++;
			hawthornPlantCollectedFlag = true;
		}
		if(player->bestiary->witchhazelPlantCollected && !witchhazelPlantCollectedFlag) {
			completedObjectives++;
			witchhazelPlantCollectedFlag = true;
		}
		if(player->bestiary->hepaticaPlantCollected && !hepaticaPlantCollectedFlag) {
			completedObjectives++;
			hepaticaPlantCollectedFlag = true;
		}

		// Calculate the completion value based on the percentage of completed objectives
		exploreForestQuest->AddCompletionValue((completedObjectives * 100) / totalObjectives);

		// Check if all objectives are completed
		if(completedObjectives == totalObjectives)
		{
			explore = true;
			LOG("Quest completed: %s", app->questManager->GetQuestById(10)->GetTitle().GetString());
			return true; // Add a return statement
		}
		return false; // Add a default return statement
	});

	if (!app->questManager->GetQuestById(10)->IsCompleted() and app->questManager->GetQuestById(7)->IsCompleted())
		exploreForestQuest->SetActive(true);

	Quest* collectItemsQuest = app->questManager->GetQuestById(11);
	collectItemsQuest->SetCompletionAction([=, this]() -> bool {
		static bool collect;
		int totalObjectives = 5; // total number of objectives
		int completedObjectives = 0; // completed objectives

		static bool arnicaPlantCollectedFlag = false;
		static bool comfreyPlantCollectedFlag = false;
		static bool hawthornPlantCollectedFlag = false;
		static bool witchhazelPlantCollectedFlag = false;
		static bool hepaticaPlantCollectedFlag = false;

		if(player->bestiary->arnicaPlantCollected && !arnicaPlantCollectedFlag) {
			completedObjectives++;
			arnicaPlantCollectedFlag = true;
		}
		if(player->bestiary->comfreyPlantCollected && !comfreyPlantCollectedFlag) {
			completedObjectives++;
			comfreyPlantCollectedFlag = true;
		}
		if(player->bestiary->hawthornPlantCollected && !hawthornPlantCollectedFlag) {
			completedObjectives++;
			hawthornPlantCollectedFlag = true;
		}
		if(player->bestiary->witchhazelPlantCollected && !witchhazelPlantCollectedFlag) {
			completedObjectives++;
			witchhazelPlantCollectedFlag = true;
		}
		if(player->bestiary->hepaticaPlantCollected && !hepaticaPlantCollectedFlag) {
			completedObjectives++;
			hepaticaPlantCollectedFlag = true;
		}

		// Calculate the completion value based on the percentage of completed objectives
		collectItemsQuest->AddCompletionValue((completedObjectives * 100) / totalObjectives);

		// Check if all objectives are completed
		if(completedObjectives == totalObjectives)
		{
			collect = true;
			LOG("Quest completed: %s", app->questManager->GetQuestById(11)->GetTitle().GetString());
			return true; // Add a return statement
		}
		return false; // Add a default return statement
	});

	if (!app->questManager->GetQuestById(11)->IsCompleted() and app->questManager->GetQuestById(7)->IsCompleted())
		collectItemsQuest->SetActive(true);

	Quest* defeatEnemiesQuest = app->questManager->GetQuestById(12);
	defeatEnemiesQuest->SetCompletionAction([=, this]() -> bool {
		static bool defeat;
		static int totalObjectives = 2; // total number of objectives
		static int completedObjectives = 0; // completed objectives

		static bool enemy1KilledFlag = false;
		static bool enemy2KilledFlag = false;

		if(player->bestiary->enemy1Killed && !enemy1KilledFlag) {
			completedObjectives++;
			enemy1KilledFlag = true;
			// Calculate the completion value based on the percentage of completed objectives
			defeatEnemiesQuest->AddCompletionValue((completedObjectives * 100) / totalObjectives);
		}
		if(player->bestiary->enemy2Killed && !enemy2KilledFlag) {
			completedObjectives++;
			enemy2KilledFlag = true;
			// Calculate the completion value based on the percentage of completed objectives
			defeatEnemiesQuest->AddCompletionValue((completedObjectives * 100) / totalObjectives);
		}

		// Check if all objectives are completed
		if(completedObjectives == totalObjectives)
		{
			defeat = true;
			LOG("Quest completed: %s", app->questManager->GetQuestById(12)->GetTitle().GetString());
			return true; // Add a return statement
		}
		return false; // Add a default return statement
	});

	if (!app->questManager->GetQuestById(12)->IsCompleted() and app->questManager->GetQuestById(7)->IsCompleted())
		defeatEnemiesQuest->SetActive(true);

	#pragma endregion Quests

	if (freeCam)
	{
		float camSpeed = 1;

		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			app->render->camera.y -= (int)ceil(camSpeed * dt);

		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			app->render->camera.y += (int)ceil(camSpeed * dt);

		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			app->render->camera.x -= (int)ceil(camSpeed * dt);

		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			app->render->camera.x += (int)ceil(camSpeed * dt);
	}

	//Cambios de escena sin collider
	//Inicio a lvl1
	if (app->entityManager->GetPlayerEntity()->position.x >= 33408 && app->entityManager->GetPlayerEntity()->position.x <= 33792 && app->entityManager->GetPlayerEntity()->position.y >= 896 && app->entityManager->GetPlayerEntity()->position.y <= 1024) {
		player->pbody->body->SetTransform({ 36,125 }, 0);
	}
	//lvl1 a lvl2
	if (app->entityManager->GetPlayerEntity()->position.x >= 6333 && app->entityManager->GetPlayerEntity()->position.x <= 6400 && app->entityManager->GetPlayerEntity()->position.y >= 2560 && app->entityManager->GetPlayerEntity()->position.y <= 3072) {
		player->pbody->body->SetTransform({ 187,30 }, 0);
	}
	//lvl2 a lvl3
	if (app->entityManager->GetPlayerEntity()->position.x >= 9600 && app->entityManager->GetPlayerEntity()->position.x <= 9666 && app->entityManager->GetPlayerEntity()->position.y >= 4224 && app->entityManager->GetPlayerEntity()->position.y <= 4736) {
		player->pbody->body->SetTransform({ 70,175 }, 0);
	}
	//lvl3 a lvl4
	if (app->entityManager->GetPlayerEntity()->position.x >= 6200 && app->entityManager->GetPlayerEntity()->position.x <= 6272 && app->entityManager->GetPlayerEntity()->position.y >= 15360 && app->entityManager->GetPlayerEntity()->position.y <= 15872) {
		player->pbody->body->SetTransform({ 344,190 }, 0);
	}
	//lvl4 a lvl5
	if (app->entityManager->GetPlayerEntity()->position.x >= 8576 && app->entityManager->GetPlayerEntity()->position.x <= 8665 && app->entityManager->GetPlayerEntity()->position.y >= 11647 && app->entityManager->GetPlayerEntity()->position.y <= 12161) {
		player->pbody->body->SetTransform({ 548,27 }, 0);
	}
	//lvl5 a lvl6
	if (app->entityManager->GetPlayerEntity()->position.x >= 20992 && app->entityManager->GetPlayerEntity()->position.x <= 21504 && app->entityManager->GetPlayerEntity()->position.y >= 384 && app->entityManager->GetPlayerEntity()->position.y <= 460) {
		player->pbody->body->SetTransform({ 380,283 }, 0);
	}
	//lvl6 a final
	if (enemyboss->dead && app->entityManager->GetPlayerEntity()->position.x >= 29111 && app->entityManager->GetPlayerEntity()->position.x <= 29184 && app->entityManager->GetPlayerEntity()->position.y >= 13696 && app->entityManager->GetPlayerEntity()->position.y <= 14208) {
		player->pbody->body->SetTransform({ 673,295 }, 0);
	}

	//puzzle1
	if (player->position.y > 4350 && player->position.x > 3776 && player->position.x < 4297 && !door1Closed && !puzzle1) {
		door1Closed = true;
		bushPbody1 = app->physics->CreateRectangle(4096, 4096, 512, 384, bodyType::STATIC);
		bushPbody1->ctype = ColliderType::LIMITS;
		int i= 4297;
		/*if(i > 3776)
		{
			app->render->DrawTexture(bush, i-1, 4000, 0, 1.0f);
		}*/
	}

	if (door1Closed) {
		app->render->DrawTexture(bush, 3776, 3953, 0, 1.0f, 0.0f, 0.6f);
	}

	if ((buttonList[4]->pisada || buttonList[5]->pisada) && !puzzle1) {
		for (int i = 0; i < 6; i++)
		{
			buttonList[i]->pisada = false;
		}
	}

	if (buttonList[0]->pisada && buttonList[1]->pisada && buttonList[2]->pisada && buttonList[3]->pisada && door1Closed && !puzzle1) {
		puzzle1 = true;
		app->audio->PlayFx(puzzleFx);
		door1Closed = false;
		app->physics->DestroyBody(bushPbody1);
		generateRandomPotion();
	}


	//puzzle 2
	
	if ((PIXEL_TO_METERS(ballList[0]->position.DistanceTo(buttonBallList[0]->position)) < 0.5f)) {
		buttonBallList[0]->pisada = true;
		ballList[0]->placed = true;
	}

	if ((PIXEL_TO_METERS(ballList[1]->position.DistanceTo(buttonBallList[1]->position)) < 0.5f)) {
		buttonBallList[1]->pisada = true;
		ballList[1]->placed = true;
	}

	if (buttonBallList[0]->pisada && buttonBallList[1]->pisada && !puzzle2) {
		puzzle2 = true;
		app->audio->PlayFx(puzzleFx);
		generateRandomPotion();
	}


	//puzzle 3

	if (player->position.x > 24340 && player->position.y > 14450 && player->position.y < 144859 && !door3Closed && !puzzle3) {
		door3Closed = true;
		bushPbody2 = app->physics->CreateRectangle(24108, 14695, 384, 512, bodyType::STATIC);
		bushPbody2->ctype = ColliderType::LIMITS;
	}

	if (door3Closed) {
		app->render->DrawTexture(bush, 23826, 14550, 0, 1.0f, 90.0f, 0.6f);
	}

	if (buttonColourList[0]->colour == 2 && buttonColourList[1]->colour == 5 && buttonColourList[2]->colour == 1 && buttonColourList[3]->colour == 4 && buttonColourList[4]->colour == 3 && door3Closed && !puzzle3) {
		puzzle3 = true;
		app->audio->PlayFx(puzzleFx);
		door3Closed = false;
		app->physics->DestroyBody(bushPbody2);
		generateRandomPotion();
	}

	//boss2

	if (player->position.x > 26900 && player->position.y > 14066 && player->position.y < 144475 && !doorBoss2Closed && !enemyboss->dead && puzzle3) {
		doorBoss2Closed = true;
		bushPbody3 = app->physics->CreateRectangle(26668, 14311, 384, 512, bodyType::STATIC);
		bushPbody3->ctype = ColliderType::LIMITS;
	}

	if (doorBoss2Closed) {
		app->render->DrawTexture(bush, 26386, 14166, 0, 1.0f, 90.0f, 0.6f);
	}

	if (doorBoss2Closed && enemyboss->dead) {
		doorBoss2Closed = false;
		app->physics->DestroyBody(bushPbody3);
	}

	//boss1

	if (player->position.x > 24340 && player->position.y > 14450 && player->position.y < 144859 && !doorBossClosed && !puzzle3) {
		doorBossClosed = true;
		bushPbody4 = app->physics->CreateRectangle(26668, 14311, 384, 512, bodyType::STATIC);
		bushPbody4->ctype = ColliderType::LIMITS;
	}

	if (doorBossClosed) {
		app->render->DrawTexture(bush, 26386, 14166, 0, 1.0f, 90.0f, 0.6f);
	}

	if (doorBossClosed && puzzle3) {
		doorBossClosed = false;
		app->physics->DestroyBody(bushPbody4);
	}


	//SHORTCUT FINAL LEVEL
	if (app->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT) {
		player->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(180*128), PIXEL_TO_METERS(113*128)), 0);
	}

	//SHORTCUT TREE FINAL SCENE
	if (app->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT) {
		player->pbody->body->SetTransform(b2Vec2(PIXEL_TO_METERS(262 * 128), PIXEL_TO_METERS(111 * 128)), 0);
	}
	
	//LOG("x: %i   y: %i", player->position.x, player->position.y);

	if (player->deadPlayer) {
		app->render->camera.lerpSpeed = 0.0f; 
		//TODO: animation player death
		if (loseScreen == nullptr) {
			loseScreen = (GuiControlPopUp*)app->guiManager->CreateGuiControl(GuiControlType::POPUP, 13, "", { (int)windowW / 2 - 960, (int)windowH / 2 - 540 }, this, loseScreenTex);
			paused = true; 
		}
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && loseScreen != nullptr) {
				paused = false;
				player->vida = player->maxVida;
				player->deadPlayer = false;
				app->guiManager->RemoveGuiControl(loseScreen);
				loseScreen = nullptr; 
				app->sceneManager->ChangeScene("tavernscene");
		}
	}

	if (!player->deadPlayer && player->winCondition) {
		app->render->camera.lerpSpeed = 0.0f;
		if (winScreen == nullptr) {
			winScreen = (GuiControlPopUp*)app->guiManager->CreateGuiControl(GuiControlType::POPUP, 13, "", { (int)windowW / 2 - 960, (int)windowH / 2 - 540 }, this, winScreenTex);
			paused = true; 
		}
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && winScreen != nullptr) {
			paused = false;
			player->vida = player->maxVida;
			player->deadPlayer = false;
			player->winCondition = false;
			app->guiManager->RemoveGuiControl(winScreen);
			winScreen = nullptr;
			app->sceneManager->ChangeScene("credits");

		}
	}

	return true;
}

// Called each loop iteration
bool ForestScene::PostUpdate()
{
	// OPTICK PROFILIN
	ZoneScoped;

	bool ret = true;

	return ret;
}

bool ForestScene::Exit()
{
	//IMPORTANTE: DESCARGAR EN ORDEN INVERSO AL CARGADO EN EL APP
	app->entityManager->Disable();
	app->map->Disable();

	return true;
}

// Called before quitting
bool ForestScene::CleanUp()
{
	app->audio->CleanUp();
	app->tex->UnLoad(bush);

	return true;
}

bool ForestScene::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Press Gui Control: %d", control->id);


	return true;
}
