
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

	return true;
}

bool UI::Update(float dt)
{
	//Potions Inventory
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT) {
		//Get the size of the window
		app->win->GetWindowSize(windowW, windowH);
		app->render->DrawTexture(texture_inventory, (int)windowW / 2 - 350, (int)windowH / 2 - 350, 0, 0);
		Inventory* playerInventory = &app->entityManager->GetPlayerEntity()->inventory;
		for(int i = 0; i < playerInventory->items.size(); i++)
		{
			Item* potion = playerInventory->items[i];
			if(potion->quantity > 0)
			{
				if(potion->name == "Celerita Potion")
				{
					if(app->input->GetMouseX() > windowW / 2 - 350 && app->input->GetMouseX() < windowW / 2 - 100 && app->input->GetMouseY() > windowH / 2 - 350 && app->input->GetMouseY() < windowH / 2 - 100)
					{
						app->render->DrawTexture(celeritaPotionSelected, (int)windowW / 2 - 250, (int)windowH / 2 - 450, 0, 0);
						if(app->input->GetMouseButtonDown(1))app->entityManager->GetPlayerEntity()->currentPotion = (Potion*)potion;
					}
					else
					{
						app->render->DrawTexture(celeritaPotionCreated, (int)windowW / 2 - 250, (int)windowH / 2 - 450, 0, 0);
					}
					app->render->DrawText(std::to_string(potion->quantity).c_str(), (int)windowW / 2 - 0, (int)windowH / 2 - 250, 50, 50, { 0, 0, 0, 255 });
				}
				else if(potion->name == "Ether Potion")
				{
					if(app->input->GetMouseX() > windowW / 2 - 350 && app->input->GetMouseX() < windowW / 2 - 100 && app->input->GetMouseY() > windowH / 2 - 350 && app->input->GetMouseY() < windowH / 2 - 100)
					{
						app->render->DrawTexture(etherPotionSelected, (int)windowW / 2 - 250, (int)windowH / 2 - 0, 0, 0);
						if(app->input->GetMouseButtonDown(1))app->entityManager->GetPlayerEntity()->currentPotion = (Potion*)potion;
					}
					else
					{
						app->render->DrawTexture(etherPotionCreated, (int)windowW / 2 - 250, (int)windowH / 2 - 0, 0, 0);
					}
				}
				else if(potion->name == "Oblitius Potion")
				{
					if(app->input->GetMouseX() > windowW / 2 - 350 && app->input->GetMouseX() < windowW / 2 - 100 && app->input->GetMouseY() > windowH / 2 - 350 && app->input->GetMouseY() < windowH / 2 - 100)
					{
						app->render->DrawTexture(oblitiusPotionSelected, (int)windowW / 2 - 0, (int)windowH / 2 - 250, 0, 0);
						if(app->input->GetMouseButtonDown(1))app->entityManager->GetPlayerEntity()->currentPotion = (Potion*)potion;
					}
					else
					{
						app->render->DrawTexture(oblitiusPotionCreated, (int)windowW / 2 - 0, (int)windowH / 2 - 250, 0, 0);
					}
				}
				else if(potion->name == "Vita Potion")
				{
					if(app->input->GetMouseX() > windowW / 2 - 350 && app->input->GetMouseX() < windowW / 2 - 100 && app->input->GetMouseY() > windowH / 2 - 350 && app->input->GetMouseY() < windowH / 2 - 100)
					{
						app->render->DrawTexture(VitaPotionSelected, (int)windowW / 2 - 450, (int)windowH / 2 - 250, 0, 0);
						if(app->input->GetMouseButtonDown(1))app->entityManager->GetPlayerEntity()->currentPotion = (Potion*)potion;
					}
					else
					{
						app->render->DrawTexture(VitaPotionCreated, (int)windowW / 2 - 450, (int)windowH / 2 - 250, 0, 0);
					}
				}
			}
		}
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
				app->render->DrawTexture(celeritaPotion, 280, 615, 0, 0);
			}
			else if(player->currentPotion->name == "Ether Potion")
			{
				app->render->DrawTexture(etherPotion, 280, 615, 0, 0);
			}
			else if(player->currentPotion->name == "Oblitius Potion")
			{
				app->render->DrawTexture(oblitiusPotion, 280, 615, 0, 0);
			}
			else if(player->currentPotion->name == "Vita Potion")
			{
				app->render->DrawTexture(VitaPotion, 280, 615, 0, 0);
			}
		}

		app->render->DrawTexture(Seleccion, 260, 615, 0, 0);

		//Dash
		float dashSelectionValue = SDL_min(player->dashTimer.ReadMSec() / 1000 / player->dashCultdown * 255, 255);
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
