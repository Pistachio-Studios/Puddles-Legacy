#include "Core/App.h"
#include "Core/Input.h"
#include "Utils/SString.h"
#include "Core/Render.h"
#include "Core/Textures.h"
#include "Utils/Timer.h"
#include "Core/Window.h"
#include "Gameplay/TavernScene.h"
#include "Gameplay/Entities/Npcs/Tabernero.h"
#include "Gameplay/Entities/Npcs/Npc.h"
#include "Gameplay/Entities/PlayerPointAndClick.h"
#include "Core/Map.h"
#include "Core/SceneManager.h"
#include "Utils/Log.h"
#include "Core/GuiControl.h"
#include "Core/GuiManager.h"
  
#include <box2d/b2_body.h>
#include <tracy/Tracy.hpp>

// Destructor
TavernScene::~TavernScene()
{}

// Called before the first frame
bool TavernScene::Enter()
{
	// iterate all objects in the testscene
	// Check https://pugixml.org/docs/quickstart.html#access

	if (parameters.child("player")) {
		player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
		player->parameters = parameters.child("player");
		player->Enable();
	}

	playerPointAndClick = new PlayerPointAndClick();
	playerPointAndClick->parameters = parameters.child("player");
	app->entityManager->AddEntity((Entity*)playerPointAndClick);
	playerPointAndClick->Enable();

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

	if (parameters.child("tabernero")) {
		Tabernero* tabernero = new Tabernero();
		app->entityManager->AddEntity(tabernero);
		tabernero->parameters = parameters.child("tabernero");
		tabernero->Start();
	}

	//app->physics->Enable();
	app->map->Enable();
	app->entityManager->Enable();

	app->render->camera.target = playerPointAndClick;
	app->render->camera.useInterpolation = true;
	app->render->camera.lerpSpeed = 2.0f;
	app->render->camera.offset = { 0,0 };

	playingTime = new Timer();

	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	//Get the size of the texture
	//app->tex->GetSize(img, texW, texH);

	cauldronTrigger = app->physics->CreateRectangle(500, 700, 100, 100, bodyType::STATIC);
	cauldronTrigger->ctype = ColliderType::CAULDRON;

	cauldronTex = app->tex->Load("Assets/Textures/Potions/Cauldron/Cauldron.png");
	cauldronSelectTex = app->tex->Load("Assets/Textures/Potions/Cauldron/CauldronSelect.png");

	CeleritaPotionTex = app->tex->Load("Assets/Textures/Potions/CreatePotion/CrafteableCeleritaPotion.png");
	NotCeleritaPotionTex = app->tex->Load("Assets/Textures/Potions/CreatePotion/NotCrafteableCeleritaPotion.png");

	EtherPotionTex = app->tex->Load("Assets/Textures/Potions/CreatePotion/CrafteableEtherPotion.png");
	NotEtherPotionTex = app->tex->Load("Assets/Textures/Potions/CreatePotion/NotCrafteableEtherPotion.png");

	VitaPotionTex = app->tex->Load("Assets/Textures/Potions/CreatePotion/CrafteableVitaPotion.png");
	NotVitaPotionTex = app->tex->Load("Assets/Textures/Potions/CreatePotion/NotCrafteableVitaPotion.png");

	OblitiusPotionTex = app->tex->Load("Assets/Textures/Potions/CreatePotion/CrafteableOblitiusPotion.png");
	NotOblitiusPotionTex = app->tex->Load("Assets/Textures/Potions/CreatePotion/NotCrafteableOblitiusPotion.png");

	currentPotion = CeleritaPotionTex;

	return true;
}

// Called each loop iteration
bool TavernScene::PreUpdate()
{
	// OPTICK PROFILIN
	ZoneScoped;

	return true;
}

// Called each loop iteration
bool TavernScene::Update(float dt)
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

	if(playerPointAndClick->cauldronIsOpen) app->render->camera.lerpSpeed = 0.0f;
	else app->render->camera.lerpSpeed = 2.0f;

	if (playerPointAndClick->cauldronIsOpen && cauldron == nullptr) {
		cauldron = (GuiControlPopUp*)app->guiManager->CreateGuiControl(GuiControlType::POPUP, 13, "test", { (int)windowW / 2 - 800, (int)windowH / 2 - 450 }, this, cauldronTex);
		cauldronExit = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 14, "Exit", { (int)windowW / 2 + 550, (int)windowH / 2 + 350, 200, 50 }, this);
		cauldronCreate = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "Create", { (int)windowW / 2 + 320, (int)windowH / 2 + 350, 200, 50 }, this);
	}

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN && cauldronSelect != nullptr) {
		if (type >= 1 && type < 4) {
			type++;
			ResetPotionPopUps();
		}
	}
	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN && cauldronSelect != nullptr) {
		if (type > 1 && type <= 4) {
			type--;
			ResetPotionPopUps();
		}
	}

	if (cauldron != nullptr) {
		if (cauldronExitPressed) {
			app->guiManager->RemoveGuiControl(cauldronCreate);
			app->guiManager->RemoveGuiControl(cauldronExit);
			app->guiManager->RemoveGuiControl(cauldron);
			cauldronExitPressed = false;
			playerPointAndClick->cauldronIsOpen = false;
			cauldron = nullptr;
		}

		if (cauldronCreatePressed && cauldronSelect == nullptr) {
			cauldronSelect = (GuiControlPopUp*)app->guiManager->CreateGuiControl(GuiControlType::POPUP, 13, "test", { (int)windowW / 2 - 800, (int)windowH / 2 - 450 }, this, cauldronSelectTex);
			cauldronSelectExit = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 12, "Exit", { (int)windowW / 2 + 550, (int)windowH / 2 + 350, 200, 50 }, this);
			potionCreateButton = (GuiControlButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 15, "Accept", { (int)windowW / 2 - 100, (int)windowH / 2 + 300, 200, 50 }, this);

			cauldronCreatePressed = false;
		}

		if (potionCreatePressed && cauldronSelect != nullptr) {

			potionCreatePressed = false;
		}

		Inventory* playerInventory = &app->entityManager->GetPlayerEntity()->inventory; 
		Item* potion; 
		if (cauldronSelect != nullptr) {
			ResetPotionPopUps();
			switch (type) {
			case 1:
				ManagePotionCreation(playerInventory, CeleritaPotionTex, NotCeleritaPotionTex, CrafteableCeleritaPotion, NotCrafteableCeleritaPotion); 
				break;
			case 2:
				ManagePotionCreation(playerInventory, EtherPotionTex, NotEtherPotionTex, CrafteableEtherPotion, NotCrafteableEtherPotion); 
				break;
			case 3:
				ManagePotionCreation(playerInventory, VitaPotionTex, NotVitaPotionTex, CrafteableVitaPotion, NotCrafteableVitaPotion); 
				break;
			case 4:
				ManagePotionCreation(playerInventory, OblitiusPotionTex, NotOblitiusPotionTex, CrafteableOblitiusPotion, NotCrafteableOblitiusPotion); 
				break;
			}
		}

		if (selectExitPressed && cauldronSelect != nullptr) {
			app->guiManager->RemoveGuiControl(cauldronSelectExit);
			app->guiManager->RemoveGuiControl(cauldronSelect);
			app->guiManager->RemoveGuiControl(potionCreateButton);
			app->tex->UnLoad(currentPotion);

			ResetPotionPopUps();

			cauldronSelect = nullptr; 
			selectExitPressed = false; 
			playerPointAndClick->cauldronIsOpen = false;
			
		}

		
	}

	//Cambios de escena sin collider
	if (app->entityManager->GetPlayerEntity()->position.x <= 1390 && app->entityManager->GetPlayerEntity()->position.x >= 1150 && app->entityManager->GetPlayerEntity()->position.y <= 3835 && app->entityManager->GetPlayerEntity()->position.y >= 3670) {
		app->sceneManager->ChangeScene("townscene");
	}

	return true;
}

// Called each loop iteration
bool TavernScene::PostUpdate()
{
	// OPTICK PROFILIN
	ZoneScoped;

	bool ret = true;

	return ret;
}

bool TavernScene::Exit()
{
	//IMPORTANTE: DESCARGAR EN ORDEN INVERSO AL CARGADO EN EL APP
	app->entityManager->Disable();
	app->map->Disable();


	SDL_ShowCursor(SDL_ENABLE);

	return true;
}

// Called before quitting
bool TavernScene::CleanUp()
{
	LOG("Freeing testscene");
	return true;
}

bool TavernScene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	switch (control->id)
	{
	case 14:
		if(cauldronSelect == nullptr) cauldronExitPressed = true;
		break;
	case 11:
		cauldronCreatePressed = true;
		break;
	case 12:
		selectExitPressed = true;
		break;
	case 15:
		potionCreatePressed = true;
		CreatePotion();
		break;
	case 10:
		app->SaveRequest();
		break;

	}

	return true;
}

void TavernScene::ResetPotionPopUps() {
	app->guiManager->RemoveGuiControl(CrafteableCeleritaPotion);
	CrafteableCeleritaPotion = nullptr;
	app->guiManager->RemoveGuiControl(CrafteableEtherPotion);
	CrafteableEtherPotion = nullptr;
	app->guiManager->RemoveGuiControl(CrafteableVitaPotion);
	CrafteableVitaPotion = nullptr;
	app->guiManager->RemoveGuiControl(CrafteableOblitiusPotion);
	CrafteableOblitiusPotion = nullptr;
	app->guiManager->RemoveGuiControl(NotCrafteableCeleritaPotion);
	NotCrafteableCeleritaPotion = nullptr;
	app->guiManager->RemoveGuiControl(NotCrafteableEtherPotion);
	NotCrafteableEtherPotion = nullptr;
	app->guiManager->RemoveGuiControl(NotCrafteableVitaPotion);
	NotCrafteableVitaPotion = nullptr;
	app->guiManager->RemoveGuiControl(NotCrafteableOblitiusPotion);
	NotCrafteableOblitiusPotion = nullptr;
}

void TavernScene::ManagePotionCreation(Inventory* playerInventory, SDL_Texture* craftableTex, SDL_Texture* notCraftableTex, GuiControlPopUp*& craftablePopup, GuiControlPopUp*& notCraftablePopup)
{
	bool hasEnoughIngredients = false;

	switch (type) {
	case 1: // CeleritaPotion
		hasEnoughIngredients = CheckIngredient(playerInventory, "Arnica Plant", 1);
		break;
	case 2: // EtherPotion
		//TODO: Segun lo que hay en el libro d las pociones, nos falta implementar las otras plantas...... (hay 4 o 5 plantas creo y tenemos 3) :) 
		break;
	case 3: // VitaPotion
		hasEnoughIngredients = CheckIngredient(playerInventory, "Arnica Plant", 3);
		break;
	case 4: // OblitiusPotion
		//TODO
		break;
	}

	if (hasEnoughIngredients) {
		if (craftablePopup == nullptr) { 
			ResetPotionPopUps(); 
			craftablePopup = (GuiControlPopUp*)app->guiManager->CreateGuiControl(GuiControlType::POPUP, 13, "test", { (int)windowW / 2 - 800, (int)windowH / 2 - 450 }, this, craftableTex);
		}
	}
	else {
		if (notCraftablePopup == nullptr) {
			ResetPotionPopUps();
			notCraftablePopup = (GuiControlPopUp*)app->guiManager->CreateGuiControl(GuiControlType::POPUP, 13, "test", { (int)windowW / 2 - 800, (int)windowH / 2 - 450 }, this, notCraftableTex);
		}
	}
}

bool TavernScene::CheckIngredient(Inventory* playerInventory, const std::string& ingredientName, int requiredQuantity)
{
	for (const auto& item : playerInventory->items) {
		if (item->name == ingredientName && item->quantity >= requiredQuantity) {
			return true;
		}
	}
	return false;
}

void TavernScene::CreatePotion()
{
	Inventory* playerInventory = &app->entityManager->GetPlayerEntity()->inventory;

	bool canCraft = false;
	std::string potionName;
	int requiredQuantity = 0;

	switch (type) {
	case 1: // CeleritaPotion
		canCraft = CheckIngredient(playerInventory, "Arnica Plant", 1);
		potionName = "Celerita Potion";
		requiredQuantity = 1;
		break;
	case 2: // EtherPotion
		//TODO: A�adir l�gica si hay ingredientes espec�ficos para EtherPotion
		break;
	case 3: // VitaPotion
		canCraft = CheckIngredient(playerInventory, "Arnica Plant", 3);
		potionName = "Vita Potion";
		requiredQuantity = 3;
		break;
	case 4: // OblitiusPotion
		// TODO: A�adir l�gica si hay ingredientes espec�ficos para OblitiusPotion
		break;
	}

	if (canCraft) {
		player->inventory.AddItem(potionName); //TODO: Arreglar!! Porque no me esta sumando la cantidad de la pocion que he creado al darle al boton de accept??? Solo falta q funcione esto
		RemoveIngredient(playerInventory, "Arnica Plant", requiredQuantity);
	}
}

void TavernScene::RemoveIngredient(Inventory* playerInventory, const std::string& ingredientName, int quantity)
{
	for (auto& item : playerInventory->items) {
		if (item->name == ingredientName) {
			item->quantity -= quantity;
			if (item->quantity < 0) {
				item->quantity = 0; //Arreglar esto
			}
			return;
		}
	}
}