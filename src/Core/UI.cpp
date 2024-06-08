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

	// Textures for the bestiary
	bestiaryPage1Texture = app->tex->Load("Assets/UI/GUI/Bestiario/Pag1/pag1.png");
	bestiaryPage2Texture = app->tex->Load("Assets/UI/GUI/Bestiario/Pag2/pag2.png");
	bestiaryPage3Texture = app->tex->Load("Assets/UI/GUI/Bestiario/Pag3/pag3.png");
	bestiaryPage4Texture = app->tex->Load("Assets/UI/GUI/Bestiario/Pag4/pag4.png");
	bestiaryPage5Texture = app->tex->Load("Assets/UI/GUI/Bestiario/Pag5/pag5.png");

	// Textures for sword abilities
	swordAbility100Texture = app->tex->Load("Assets/UI/GUI/Bestiario/Pag1/sword/sword1-0-0.png");
	swordAbility110Texture = app->tex->Load("Assets/UI/GUI/Bestiario/Pag1/sword/sword1-1-0.png");
	swordAbility111Texture = app->tex->Load("Assets/UI/GUI/Bestiario/Pag1/sword/sword1-1-1.png");
	swordAbility112Texture = app->tex->Load("Assets/UI/GUI/Bestiario/Pag1/sword/sword1-1-2.png");
	swordAbility120Texture = app->tex->Load("Assets/UI/GUI/Bestiario/Pag1/sword/sword1-2-0.png");
	swordAbility122Texture = app->tex->Load("Assets/UI/GUI/Bestiario/Pag1/sword/sword1-2-2.png");
	swordAbility123Texture = app->tex->Load("Assets/UI/GUI/Bestiario/Pag1/sword/sword1-2-3.png");

	// Textures for staff abilities
	staffAbility100Texture = app->tex->Load("Assets/UI/GUI/Bestiario/Pag1/staff/staff1-0-0.png");
	staffAbility110Texture = app->tex->Load("Assets/UI/GUI/Bestiario/Pag1/staff/staff1-1-0.png");
	staffAbility111Texture = app->tex->Load("Assets/UI/GUI/Bestiario/Pag1/staff/staff1-1-1.png");
	staffAbility112Texture = app->tex->Load("Assets/UI/GUI/Bestiario/Pag1/staff/staff1-1-2.png");
	staffAbility120Texture = app->tex->Load("Assets/UI/GUI/Bestiario/Pag1/staff/staff1-2-0.png");
	staffAbility122Texture = app->tex->Load("Assets/UI/GUI/Bestiario/Pag1/staff/staff1-2-2.png");
	staffAbility123Texture = app->tex->Load("Assets/UI/GUI/Bestiario/Pag1/staff/staff1-2-3.png");


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
				int potionX = (int)windowW / 2 - 252;
				int potionY = (int)windowH / 2 - 446;
				int potionWidth = 250;
				int potionHeight = 250;

				if(potion->name == "Celerita Potion")
				{
					if(app->input->GetMouseX() > potionX + 80 && app->input->GetMouseX() < potionX + potionWidth + 50 && app->input->GetMouseY() > potionY && app->input->GetMouseY() < potionY + potionHeight)
					{
						app->render->DrawTexture(celeritaPotionSelected, potionX, potionY, 0, 0);
						if(app->input->GetMouseButtonDown(1))app->entityManager->GetPlayerEntity()->currentPotion = (Potion*)potion;
					}
					else
					{
						app->render->DrawTexture(celeritaPotionCreated, potionX, potionY, 0, 0);
					}
					app->render->DrawText(std::to_string(potion->quantity).c_str(), (int)windowW / 2 + 40, (int)windowH / 2 - 250, 50, 50, { 0, 0, 0, 255 });
				}
				else if(potion->name == "Ether Potion")
				{
					int potionX = (int)windowW / 2 - 259;
					potionY = (int)windowH / 2 - 12;
					if(app->input->GetMouseX() > potionX + 80 && app->input->GetMouseX() < potionX + potionWidth + 50 && app->input->GetMouseY() > potionY && app->input->GetMouseY() < potionY + potionHeight)
					{
						app->render->DrawTexture(etherPotionSelected, potionX, potionY, 0, 0);
						if(app->input->GetMouseButtonDown(1))app->entityManager->GetPlayerEntity()->currentPotion = (Potion*)potion;
					}
					else
					{
						app->render->DrawTexture(etherPotionCreated, potionX, potionY, 0, 0);
					}
					app->render->DrawText(std::to_string(potion->quantity).c_str(), (int)windowW / 2 + 40, (int)windowH / 2 + 170, 50, 50, { 0, 0, 0, 255 });
				}
				else if(potion->name == "Oblitius Potion")
				{
					potionX = (int)windowW / 2 - 50;
					potionY = (int)windowH / 2 - 259;
					if(app->input->GetMouseX() > potionX + 80 && app->input->GetMouseX() < potionX + potionWidth + 50 && app->input->GetMouseY() > potionY && app->input->GetMouseY() < potionY + potionHeight + 30)
					{
						app->render->DrawTexture(oblitiusPotionSelected, potionX, potionY - 2, 0, 0);
						if(app->input->GetMouseButtonDown(1))app->entityManager->GetPlayerEntity()->currentPotion = (Potion*)potion;
					}
					else
					{
						app->render->DrawTexture(oblitiusPotionCreated, potionX, potionY, 0, 0);
					}
					app->render->DrawText(std::to_string(potion->quantity).c_str(), (int)windowW / 2 + 230, (int)windowH / 2 - 100, 50, 50, { 0, 0, 0, 255 });
				}
				else if(potion->name == "Vita Potion")
				{
					potionX = (int)windowW / 2 - 481;
					potionY = (int)windowH / 2 - 255;
					if(app->input->GetMouseX() > potionX + 80 && app->input->GetMouseX() < potionX + potionWidth + 50 && app->input->GetMouseY() > potionY && app->input->GetMouseY() < potionY + potionHeight + 30)
					{
						app->render->DrawTexture(VitaPotionSelected, potionX - 2, potionY, 0, 0);
						if(app->input->GetMouseButtonDown(1))app->entityManager->GetPlayerEntity()->currentPotion = (Potion*)potion;
					}
					else
					{
						app->render->DrawTexture(VitaPotionCreated, potionX, potionY, 0, 0);
					}
					app->render->DrawText(std::to_string(potion->quantity).c_str(), (int)windowW / 2 - 230, (int)windowH / 2 - 100, 50, 50, { 0, 0, 0, 255 });
				}
			}
		}
	}

	//Player UI
	Player* player = app->entityManager->GetPlayerEntity();

	if(player != nullptr){

		//Menu Pequeño
		#pragma region MenuPequeño
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
		#pragma endregion MenuPequeño

		//Dash
		#pragma region Dash
		float dashSelectionValue = SDL_min(player->dashTimer.ReadMSec() / 1000 / player->dashCultdown * 255, 255);
		SDL_SetTextureAlphaMod(Seleccion, dashSelectionValue);
		app->render->DrawTexture(Seleccion, 280, 695, 0, 0);
		SDL_SetTextureAlphaMod(Seleccion, 0);

		app->render->DrawTexture(Seleccion, 235, 770, 0, 0);
		#pragma endregion Dash

		//Health Bar
		#pragma region HealthBar
		int health = player->livesPlayer;
		app->render->DrawRectangle({ 200, 80, 295 * health / 10, 20 }, 33, 187, 129, 255, true, false);
		#pragma endregion HealthBar

		//Mana Bar
		#pragma region ManaBar
		int mana = player->mana;
		app->render->DrawRectangle({ 200, 105, 295 * mana / 100, 20 }, 62, 94, 170, 255, true, false);

		app->render->DrawTextureLegacy(VidaBestiario, 50, 25, 0, 0);
		#pragma endregion ManaBar

		//Weapons
		#pragma region Weapons
		switch(player->currentClass)
		{
		case PlayerClass::KNIGHT:
			app->render->DrawTexture(Espada, 150, 600, 0, 0);
			break;
		case PlayerClass::WIZARD:
			app->render->DrawTexture(Cetro, 150, 600, 0, 0);
			break;
		}
		#pragma endregion Weapons

		//Bestiary
		#pragma region Bestiary
		if (buttonPag1 == nullptr)
			buttonPag1 = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "", { 332,267, 60, 60 }, (Module*)this);
		if (buttonPag2 == nullptr)
			buttonPag2 = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "", { 332,350, 60, 60 }, (Module*)this);
		if (buttonPag3 == nullptr)
			buttonPag3 = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "", { 332,436, 60, 60 }, (Module*)this);
		if (buttonPag4 == nullptr)
			buttonPag4 = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "", { 332,519, 60, 60 }, (Module*)this);
		if (buttonPag5 == nullptr)
			buttonPag5 = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "", { 332,602, 60, 60 }, (Module*)this);

		// Abilitie buttons
		#pragma region AbilitiesButtons
		if (buttonSwordAbility100 == nullptr)
			buttonSwordAbility100 = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 6, "",  { 600, 700, 50, 50 }, (Module*)this);
		if (buttonSwordAbility110 == nullptr)
			buttonSwordAbility110 = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, "",  { 523, 658, 50, 50 }, (Module*)this);
		if (buttonSwordAbility111 == nullptr)
			buttonSwordAbility111 = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 8, "",  { 440, 617, 50, 50 }, (Module*)this);
		if (buttonSwordAbility112 == nullptr)
			buttonSwordAbility112 = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 9, "",  { 440, 698, 50, 50 }, (Module*)this);
		if (buttonSwordAbility120 == nullptr)
			buttonSwordAbility120 = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 10, "", { 527, 761, 50, 50 }, (Module*)this);
		if (buttonSwordAbility122 == nullptr)
			buttonSwordAbility122 = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 11, "", { 440, 698, 50, 50 }, (Module*)this);
		if (buttonSwordAbility123 == nullptr)
			buttonSwordAbility123 = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 12, "", { 437, 780, 50, 50 }, (Module*)this);
		
		if (buttonStaffAbility100 == nullptr)
			buttonStaffAbility100 = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 13, "", { 698, 700, 50, 50 }, (Module*)this);
		if (buttonStaffAbility110 == nullptr)
			buttonStaffAbility110 = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 14, "", { 778, 664, 50, 50 }, (Module*)this);
		if (buttonStaffAbility111 == nullptr)
			buttonStaffAbility111 = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 15, "", { 861, 623, 50, 50 }, (Module*)this);
		if (buttonStaffAbility112 == nullptr)
			buttonStaffAbility112 = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 16, "", { 857, 702, 50, 50 }, (Module*)this);
		if (buttonStaffAbility120 == nullptr)
			buttonStaffAbility120 = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 17, "", { 774, 759, 50, 50 }, (Module*)this);
		if (buttonStaffAbility122 == nullptr)
			buttonStaffAbility122 = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 18, "", { 857, 702, 50, 50 }, (Module*)this);
		if (buttonStaffAbility123 == nullptr)
			buttonStaffAbility123 = app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 19, "", { 857, 783, 50, 50 }, (Module*)this);

		#pragma endregion AbilitiesButtons

		if (app->input->GetKey(SDL_SCANCODE_B) == KEY_DOWN) 
			openBestiary = !openBestiary;

		if (openBestiary) {
			switch (player->bestiary->currentPage) {
				case 1:
					app->render->DrawTextureLegacy(bestiaryPage1Texture, 0, 0, NULL, 0);

					#pragma region AbilitiesTextures
					if (player->bestiary->swordAbility100Unlocked)
						app->render->DrawTextureLegacy(swordAbility100Texture, 0, 0, NULL, 0);

					if (player->bestiary->swordAbility110Unlocked)
						app->render->DrawTextureLegacy(swordAbility110Texture, 0, 0, NULL, 0);

					if (player->bestiary->swordAbility111Unlocked)
						app->render->DrawTextureLegacy(swordAbility111Texture, 0, 0, NULL, 0);

					if (player->bestiary->swordAbility112Unlocked)
						app->render->DrawTextureLegacy(swordAbility112Texture, 0, 0, NULL, 0);
					
					if (player->bestiary->swordAbility120Unlocked)
						app->render->DrawTextureLegacy(swordAbility120Texture, 0, 0, NULL, 0);
					
					if (player->bestiary->swordAbility122Unlocked)
						app->render->DrawTextureLegacy(swordAbility122Texture, 0, 0, NULL, 0);

					if (player->bestiary->swordAbility123Unlocked)
						app->render->DrawTextureLegacy(swordAbility123Texture, 0, 0, NULL, 0);

					if (player->bestiary->staffAbility100Unlocked)
						app->render->DrawTextureLegacy(staffAbility100Texture, 0, 0, NULL, 0);

					if (player->bestiary->staffAbility110Unlocked)
						app->render->DrawTextureLegacy(staffAbility110Texture, 0, 0, NULL, 0);

					if (player->bestiary->staffAbility111Unlocked)
						app->render->DrawTextureLegacy(staffAbility111Texture, 0, 0, NULL, 0);

					if (player->bestiary->staffAbility112Unlocked)
						app->render->DrawTextureLegacy(staffAbility112Texture, 0, 0, NULL, 0);

					if (player->bestiary->staffAbility120Unlocked)
						app->render->DrawTextureLegacy(staffAbility120Texture, 0, 0, NULL, 0);

					if (player->bestiary->staffAbility122Unlocked)
						app->render->DrawTextureLegacy(staffAbility122Texture, 0, 0, NULL, 0);

					if (player->bestiary->staffAbility123Unlocked)
						app->render->DrawTextureLegacy(staffAbility123Texture, 0, 0, NULL, 0);

					#pragma endregion AbilitiesTextures

					break;
				case 2:
					app->render->DrawTextureLegacy(bestiaryPage2Texture, 0, 0, NULL, 0);
					break;
				case 3:
					app->render->DrawTextureLegacy(bestiaryPage3Texture, 0, 0, NULL, 0);
					break;
				case 4:
					app->render->DrawTextureLegacy(bestiaryPage4Texture, 0, 0, NULL, 0);
					break;
				case 5:
					app->render->DrawTextureLegacy(bestiaryPage5Texture, 0, 0, NULL, 0);
					break;
				}
		};
		#pragma endregion Bestiary
	}
	
	return true;
}


bool UI::CleanUp() {

	#pragma region CleanUp
	app->tex->UnLoad(texture_inventory);
	app->tex->UnLoad(celeritaPotionCreated);
	app->tex->UnLoad(celeritaPotionSelected);
	app->tex->UnLoad(etherPotionCreated);
	app->tex->UnLoad(etherPotionSelected);
	app->tex->UnLoad(oblitiusPotionCreated);
	app->tex->UnLoad(oblitiusPotionSelected);
	app->tex->UnLoad(VitaPotionCreated);
	app->tex->UnLoad(VitaPotionSelected);

	app->tex->UnLoad(MenuPequeno);
	app->tex->UnLoad(celeritaPotion);
	app->tex->UnLoad(etherPotion);
	app->tex->UnLoad(oblitiusPotion);
	app->tex->UnLoad(VitaPotion);

	app->tex->UnLoad(VidaBestiario);
	app->tex->UnLoad(Espada);
	app->tex->UnLoad(Cetro);
	app->tex->UnLoad(Seleccion);

	app->tex->UnLoad(bestiaryPage1Texture);
	app->tex->UnLoad(bestiaryPage2Texture);
	app->tex->UnLoad(bestiaryPage3Texture);
	app->tex->UnLoad(bestiaryPage4Texture);
	app->tex->UnLoad(bestiaryPage5Texture);

	app->guiManager->RemoveGuiControl(buttonPag1);
	app->guiManager->RemoveGuiControl(buttonPag2);
	app->guiManager->RemoveGuiControl(buttonPag3);
	app->guiManager->RemoveGuiControl(buttonPag4);
	app->guiManager->RemoveGuiControl(buttonPag5);
	buttonPag1 = nullptr;
	buttonPag2 = nullptr;
	buttonPag3 = nullptr;
	buttonPag4 = nullptr;
	buttonPag5 = nullptr;

	app->tex->UnLoad(swordAbility100Texture);
	app->tex->UnLoad(swordAbility110Texture);
	app->tex->UnLoad(swordAbility111Texture);
	app->tex->UnLoad(swordAbility112Texture);
	app->tex->UnLoad(swordAbility120Texture);
	app->tex->UnLoad(swordAbility122Texture);
	app->tex->UnLoad(swordAbility123Texture);

	app->tex->UnLoad(staffAbility100Texture);
	app->tex->UnLoad(staffAbility110Texture);
	app->tex->UnLoad(staffAbility111Texture);
	app->tex->UnLoad(staffAbility112Texture);
	app->tex->UnLoad(staffAbility120Texture);
	app->tex->UnLoad(staffAbility122Texture);
	app->tex->UnLoad(staffAbility123Texture);

	app->guiManager->RemoveGuiControl(buttonSwordAbility100);
	app->guiManager->RemoveGuiControl(buttonSwordAbility110);
	app->guiManager->RemoveGuiControl(buttonSwordAbility111);
	app->guiManager->RemoveGuiControl(buttonSwordAbility112);
	app->guiManager->RemoveGuiControl(buttonSwordAbility120);
	app->guiManager->RemoveGuiControl(buttonSwordAbility122);
	app->guiManager->RemoveGuiControl(buttonSwordAbility123);
	buttonSwordAbility100 = nullptr;
	buttonSwordAbility110 = nullptr;
	buttonSwordAbility111 = nullptr;
	buttonSwordAbility112 = nullptr;
	buttonSwordAbility120 = nullptr;
	buttonSwordAbility122 = nullptr;
	buttonSwordAbility123 = nullptr;

	app->guiManager->RemoveGuiControl(buttonStaffAbility100);
	app->guiManager->RemoveGuiControl(buttonStaffAbility110);
	app->guiManager->RemoveGuiControl(buttonStaffAbility111);
	app->guiManager->RemoveGuiControl(buttonStaffAbility112);
	app->guiManager->RemoveGuiControl(buttonStaffAbility120);
	app->guiManager->RemoveGuiControl(buttonStaffAbility122);
	app->guiManager->RemoveGuiControl(buttonStaffAbility123);
	buttonStaffAbility100 = nullptr;
	buttonStaffAbility110 = nullptr;
	buttonStaffAbility111 = nullptr;
	buttonStaffAbility112 = nullptr;
	buttonStaffAbility120 = nullptr;
	buttonStaffAbility122 = nullptr;
	buttonStaffAbility123 = nullptr;
	#pragma endregion CleanUp

	return true;
}

bool UI::OnGuiMouseClickEvent(GuiControl* control)
{
	if (control->type == GuiControlType::BUTTON)
	{
		switch (control->id)
		{
		case 1:
			app->entityManager->GetPlayerEntity()->bestiary->setPage(1);
			break;
		case 2:
			app->entityManager->GetPlayerEntity()->bestiary->setPage(2);
			break;
		case 3:
			app->entityManager->GetPlayerEntity()->bestiary->setPage(3);
			break;
		case 4:
			app->entityManager->GetPlayerEntity()->bestiary->setPage(4);
			break;
		case 5:
			app->entityManager->GetPlayerEntity()->bestiary->setPage(5);
			break;
		case 6:
			app->entityManager->GetPlayerEntity()->bestiary->swordAbility100Unlocked = true;

			// disable other sword abilities
			app->entityManager->GetPlayerEntity()->bestiary->swordAbility110Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->swordAbility111Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->swordAbility112Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->swordAbility120Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->swordAbility122Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->swordAbility123Unlocked = false;
			break;
		case 7:
			app->entityManager->GetPlayerEntity()->bestiary->swordAbility110Unlocked = true;

			// disable other sword abilities
			app->entityManager->GetPlayerEntity()->bestiary->swordAbility100Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->swordAbility111Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->swordAbility112Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->swordAbility120Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->swordAbility122Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->swordAbility123Unlocked = false;
			break;
		case 8:
			if (app->entityManager->GetPlayerEntity()->bestiary->swordAbility110Unlocked) {
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility111Unlocked = true;

				// disable other sword abilities
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility100Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility110Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility112Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility120Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility122Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility123Unlocked = false;

			}
			break;
		case 9:
			if (app->entityManager->GetPlayerEntity()->bestiary->swordAbility110Unlocked) {
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility112Unlocked = true;

				// disable other sword abilities
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility100Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility110Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility111Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility120Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility122Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility123Unlocked = false;		

			}
			break;
			
		case 10:
			app->entityManager->GetPlayerEntity()->bestiary->swordAbility120Unlocked = true;

			// disable other sword abilities
			app->entityManager->GetPlayerEntity()->bestiary->swordAbility100Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->swordAbility110Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->swordAbility111Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->swordAbility112Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->swordAbility122Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->swordAbility123Unlocked = false;
			break;
		case 11:
			if (app->entityManager->GetPlayerEntity()->bestiary->swordAbility120Unlocked) {
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility122Unlocked = true;

				// disable other sword abilities
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility100Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility110Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility111Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility112Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility120Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility123Unlocked = false;

			}
			break;
		case 12:
			if (app->entityManager->GetPlayerEntity()->bestiary->swordAbility120Unlocked) {
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility123Unlocked = true;

				// disable other sword abilities
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility100Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility110Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility111Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility112Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility120Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->swordAbility122Unlocked = false;

			}
			break;
		case 13:
			app->entityManager->GetPlayerEntity()->bestiary->staffAbility100Unlocked = true;

			// disable other staff abilities
			app->entityManager->GetPlayerEntity()->bestiary->staffAbility110Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->staffAbility111Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->staffAbility112Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->staffAbility120Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->staffAbility122Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->staffAbility123Unlocked = false;
			break;
		case 14:
			app->entityManager->GetPlayerEntity()->bestiary->staffAbility110Unlocked = true;

			// disable other staff abilities
			app->entityManager->GetPlayerEntity()->bestiary->staffAbility100Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->staffAbility111Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->staffAbility112Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->staffAbility120Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->staffAbility122Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->staffAbility123Unlocked = false;
			break;
		case 15:
			if (app->entityManager->GetPlayerEntity()->bestiary->staffAbility110Unlocked) {
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility111Unlocked = true;

				// disable other staff abilities
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility100Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility110Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility112Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility120Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility122Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility123Unlocked = false;

			}
			break;
		case 16:
			if (app->entityManager->GetPlayerEntity()->bestiary->staffAbility110Unlocked) {
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility112Unlocked = true;

				// disable other staff abilities
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility100Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility110Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility111Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility120Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility122Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility123Unlocked = false;

			}
			break;
		case 17:
			app->entityManager->GetPlayerEntity()->bestiary->staffAbility120Unlocked = true;

			// disable other staff abilities
			app->entityManager->GetPlayerEntity()->bestiary->staffAbility100Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->staffAbility110Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->staffAbility111Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->staffAbility112Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->staffAbility122Unlocked = false;
			app->entityManager->GetPlayerEntity()->bestiary->staffAbility123Unlocked = false;
			break;
		case 18:
			if (app->entityManager->GetPlayerEntity()->bestiary->staffAbility120Unlocked) {
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility122Unlocked = true;

				// disable other staff abilities
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility100Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility110Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility111Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility112Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility120Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility123Unlocked = false;

			}
			break;
		case 19:
			if (app->entityManager->GetPlayerEntity()->bestiary->staffAbility120Unlocked) {
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility123Unlocked = true;

				// disable other staff abilities
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility100Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility110Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility111Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility112Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility120Unlocked = false;
				app->entityManager->GetPlayerEntity()->bestiary->staffAbility122Unlocked = false;

			}
			break;
		}
	}
	return true;
}
