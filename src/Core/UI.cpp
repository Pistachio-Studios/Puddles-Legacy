
#include "Core/App.h"
#include "Core/Textures.h"
#include "Core/Input.h"
#include "Core/Render.h"
#include "Core/GuiManager.h"
#include "Gameplay/Scene.h"
#include "Gameplay/Entities/Items/Potion.h"
#include "Core/EntityManager.h"
#include "Gameplay/Entities/Player.h"
#include "Utils/Point.h"
#include "Core/Physics.h"
#include "Core/Window.h"
#include "Core/UI.h"
#include "Utils/Easings.h"


UI::UI()
{
	name.Create("UI");
}

UI::UI(bool startEnabled) : Module(startEnabled)
{
	name.Create("UI");
}

UI::~UI() {

}

bool UI::Awake() {

	return true;
}

bool UI::Start() {

	texture_inventory = app->tex->Load("Assets/Textures/Potions/Inventory/noPotions.png");
	//TODO: PONER BIEN QUE LA TEXTURA DE LAS POCIONES SEAN LAS QUE CORRESPONDAN!!!! (Huguito)
	celeritaPotionCreated = app->tex->Load("Assets/Textures/Potions/Inventory/EnergyPotionCreated.png");
	etherPotionCreated = app->tex->Load("Assets/Textures/Potions/Inventory/AbilityPotionCreated.png");
	oblitiusPotionCreated = app->tex->Load("Assets/Textures/Potions/Inventory/VeloPotionCreated.png");
	VitaPotionCreated = app->tex->Load("Assets/Textures/Potions/Inventory/HealPotionCreated.png");

	celeritaPotionSelected = app->tex->Load("Assets/Textures/Potions/Inventory/EnergyPotionSelected.png");
	etherPotionSelected = app->tex->Load("Assets/Textures/Potions/Inventory/AbilityPotionSelected.png");
	oblitiusPotionSelected = app->tex->Load("Assets/Textures/Potions/Inventory/VeloPotionSelected.png");
	VitaPotionSelected = app->tex->Load("Assets/Textures/Potions/Inventory/HealPotionSelected.png");


	//Player UIGUI/
	MenuPequeno = app->tex->Load("Assets/UI/GUI/MenuPequeno.PNG");
	celeritaPotion = app->tex->Load("Assets/Textures/Potions/EnergyPotion.png");
	etherPotion = app->tex->Load("Assets/Textures/Potions/AbilityPotion.PNG");
	oblitiusPotion = app->tex->Load("Assets/Textures/Potions/VeloPotion.png");
	VitaPotion = app->tex->Load("Assets/Textures/Potions/HealPotion.png");

	VidaBestiario = app->tex->Load("Assets/UI/GUI/VidaBestiario.PNG");
	Espada = app->tex->Load("Assets/UI/GUI/Armas/Espada.PNG");
	Cetro = app->tex->Load("Assets/UI/GUI/Armas/Cetro.PNG");
	Seleccion = app->tex->Load("Assets/UI/GUI/Seleccion.PNG");

	potionEaseTimer = Timer();

	return true;
}

bool UI::Update(float dt)
{
	//Potions Inventory
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT) {

		Easings easings = Easings();		
		float sizeEase = easings.easeInQuad(potionEaseTimer.ReadMSec() / 200); //hint: from 0 to 1
		LOG("sizeEase %f", sizeEase);

		//Get the size of the window
		app->win->GetWindowSize(windowW, windowH);
		app->render->DrawTexture(texture_inventory, (int)windowW / 2 - 350 * sizeEase, (int)windowH / 2 - 350 * sizeEase, 0, 0, 0, sizeEase);
		//FixThis- SDL_SetTextureAlphaMod(texture_inventory, easeInQuad(dt));
		Inventory* playerInventory = &app->entityManager->GetPlayerEntity()->inventory;
		for(int i = 0; i < playerInventory->items.size(); i++)
		{
			Item* potion = playerInventory->items[i];
			if(potion->quantity > 0)
			{
				int potionX = (int)windowW / 2 - 252 * sizeEase;
				int potionY = (int)windowH / 2 - 446 * sizeEase;
				int potionWidth = 250;
				int potionHeight = 250;

				if(potion->name == "Celerita Potion")
				{
					if(app->input->GetMouseX() > potionX + 80 && app->input->GetMouseX() < potionX + potionWidth + 50 && app->input->GetMouseY() > potionY && app->input->GetMouseY() < potionY + potionHeight)
					{
						app->render->DrawTexture(celeritaPotionSelected, potionX, potionY, 0, 0, 0, sizeEase);
						if(app->input->GetMouseButtonDown(1))app->entityManager->GetPlayerEntity()->currentPotion = (Potion*)potion;
					}
					else
					{
						app->render->DrawTexture(celeritaPotionCreated, potionX, potionY, 0, 0, 0, sizeEase);
					}
					app->render->DrawText(std::to_string(potion->quantity).c_str(), (int)windowW / 2 + 40, (int)windowH / 2 - 250, 50, 50, { 0, 0, 0, 255 });
				}
				else if(potion->name == "Ether Potion")
				{
					int potionX = (int)windowW / 2 - 259;
					potionY = (int)windowH / 2 - 12;
					if(app->input->GetMouseX() > potionX + 80 && app->input->GetMouseX() < potionX + potionWidth + 50 && app->input->GetMouseY() > potionY && app->input->GetMouseY() < potionY + potionHeight)
					{
						app->render->DrawTexture(etherPotionSelected, potionX, potionY, 0, 0, 0, sizeEase);
						if(app->input->GetMouseButtonDown(1))app->entityManager->GetPlayerEntity()->currentPotion = (Potion*)potion;
					}
					else
					{
						app->render->DrawTexture(etherPotionCreated, potionX, potionY, 0, 0, 0, sizeEase);
					}
					app->render->DrawText(std::to_string(potion->quantity).c_str(), (int)windowW / 2 + 40, (int)windowH / 2 + 170, 50, 50, { 0, 0, 0, 255 });
				}
				else if(potion->name == "Oblitius Potion")
				{
					potionX = (int)windowW / 2 - 50;
					potionY = (int)windowH / 2 - 259;
					if(app->input->GetMouseX() > potionX + 80 && app->input->GetMouseX() < potionX + potionWidth + 50 && app->input->GetMouseY() > potionY && app->input->GetMouseY() < potionY + potionHeight + 30)
					{
						app->render->DrawTexture(oblitiusPotionSelected, potionX, potionY - 2, 0, 0, 0, sizeEase);
						if(app->input->GetMouseButtonDown(1))app->entityManager->GetPlayerEntity()->currentPotion = (Potion*)potion;
					}
					else
					{
						app->render->DrawTexture(oblitiusPotionCreated, potionX, potionY, 0, 0, 0, sizeEase);
					}
					app->render->DrawText(std::to_string(potion->quantity).c_str(), (int)windowW / 2 + 230, (int)windowH / 2 - 100, 50, 50, { 0, 0, 0, 255 });
				}
				else if(potion->name == "Vita Potion")
				{
					potionX = (int)windowW / 2 - 481;
					potionY = (int)windowH / 2 - 255;
					if(app->input->GetMouseX() > potionX + 80 && app->input->GetMouseX() < potionX + potionWidth + 50 && app->input->GetMouseY() > potionY && app->input->GetMouseY() < potionY + potionHeight + 30)
					{
						app->render->DrawTexture(VitaPotionSelected, potionX - 2, potionY, 0, 0, 0, sizeEase);
						if(app->input->GetMouseButtonDown(1))app->entityManager->GetPlayerEntity()->currentPotion = (Potion*)potion;
					}
					else
					{
						app->render->DrawTexture(VitaPotionCreated, potionX, potionY, 0, 0, 0, sizeEase);
					}
					app->render->DrawText(std::to_string(potion->quantity).c_str(), (int)windowW / 2 - 230, (int)windowH / 2 - 100, 50, 50, { 0, 0, 0, 255 });
				}
			}
		}
	}
	else
	{
		potionEaseTimer.Start();
	}

	//Player UI
	Player* player = app->entityManager->GetPlayerEntity();

	if(player != nullptr){

		//Menu Pequeño
		app->render->DrawTexture(MenuPequeno, 275, 650, 0, 0);
		if(player->currentPotion != nullptr)
		{
			if(player->currentPotion->name == "Celerita Potion")
			{
				app->render->DrawTexture(celeritaPotion, 280, 820, 0, 0, 0, 0.75f);
			}
			else if(player->currentPotion->name == "Ether Potion")
			{
				app->render->DrawTexture(etherPotion, 280, 820, 0, 0, 0, 0.75f);
			}
			else if(player->currentPotion->name == "Oblitius Potion")
			{
				app->render->DrawTexture(oblitiusPotion, 280, 820, 0, 0, 0, 0.75f);
			}
			else if(player->currentPotion->name == "Vita Potion")
			{
				app->render->DrawTexture(VitaPotion, 280, 820, 0, 0, 0, 0.75f);
			}
		}

		app->render->DrawTexture(Seleccion, 260, 615, 0, 0);

		//Dash
		Easings easings1 = Easings();
		float test = easings1.easeInQuad(player->dashTimer.ReadMSec() / 1000);
		float dashSelectionValue = SDL_min(test * 255, 255);
		SDL_SetTextureAlphaMod(Seleccion, dashSelectionValue);
		app->render->DrawTexture(Seleccion, 280, 695, 0, 0);
		SDL_SetTextureAlphaMod(Seleccion, 0);

		app->render->DrawTexture(Seleccion, 235, 770, 0, 0);

		//Health Bar
		int health = player->livesPlayer;
		app->render->DrawRectangle({ 200, 80, 295 * health / 10, 20 }, 33, 187, 129, 255, true, false);
		//Mana Bar
		int mana = player->mana;
		app->render->DrawRectangle({ 200, 105, 295 * mana / 100, 20 }, 62, 94, 170, 255, true, false);

		app->render->DrawTexture(VidaBestiario, 50, 25, 0, 0);

		//Weapons
		switch(player->currentClass)
		{
		case PlayerClass::KNIGHT:
			app->render->DrawTexture(Espada, 150, 600, 0, 0);
			break;
		case PlayerClass::WIZARD:
			app->render->DrawTexture(Cetro, 150, 600, 0, 0);
			break;
		}
	}

	return true;
}


bool UI::CleanUp() {

	return true;
}