
#include "Core/App.h"
#include "Core/Textures.h"
#include "Core/Input.h"
#include "Core/Render.h"
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

	//Player UIGUI/
	MenuPequeno = app->tex->Load("Assets/UI/GUI/MenuPequeno.PNG");
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
	}

	//Player UI
	Player* player = app->entityManager->GetPlayerEntity();

	if(player != nullptr){

		//Menu Pequeño
		app->render->DrawTexture(MenuPequeno, 275, 650, 0, 0);

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

		SDL_SetRenderTarget(app->render->renderer, app->render->overlayTarget);
		app->render->DrawTextureLegacy(VidaBestiario, 50, 25, 0, 0);
		SDL_SetRenderTarget(app->render->renderer, NULL);

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
