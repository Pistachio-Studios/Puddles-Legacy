
#include "Core/App.h"
#include "Core/Textures.h"
#include "Core/Input.h"
#include "Core/Render.h"
#include "Gameplay/Scene.h"
#include "Gameplay/Entities/Items/Potion.h"
#include "Core/EntityManager.h"
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

	texture_inventory = app->tex->Load("Assets/Textures/Potions/Inventory/NoPotions.png");

	return true;
}

bool UI::Update(float dt)
{
	//Potions Inventory
	if (app->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT) {
		//Get the size of the window
		app->win->GetWindowSize(windowW, windowH);
		app->render->DrawTexture(texture_inventory, (int)windowW / 2 - 350, (int)windowH / 2 - 350, 0, 0);

		app->entityManager->PotionUpdate(dt); 
	}

	return true;
}


bool UI::CleanUp() {

	return true;
}
