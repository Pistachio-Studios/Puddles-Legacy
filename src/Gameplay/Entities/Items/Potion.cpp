#include "Gameplay/Entities/Items/Potion.h"

#include "Core/Audio.h"

#include "Utils/Log.h"

#include "Core/App.h"
#include "Gameplay/Entities/Entity.h"
#include "Gameplay/Entities/Player.h"
#include "Core/Textures.h"
#include "Core/Input.h"
#include "Core/Render.h"
#include "Gameplay/Scene.h"
#include "Utils/Point.h"
#include "Core/Physics.h"
#include "Utils/StateMachine.h"
#include "Core/Window.h"
#include "Core/SceneManager.h"
#include "Core/Map.h"

#include <cstdlib>


Potion::Potion() : Entity(EntityType::POTION)
{
	name.Create("potion");
}

Potion::~Potion() {}

bool Potion::Awake() {

	return true;
}

bool Potion::Start() {

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	type = parameters.attribute("type").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	textureSelection = parameters.attribute("textureSelection").as_string();

	texture = app->tex->Load(texturePath);
	texture1 = app->tex->Load("Assets/Textures/Potions/Inventory/NoPotions.png");
	texture2 = app->tex->Load(textureSelection);

	pbody = app->physics->CreateRectangle(position.x, position.y, 80, 80, bodyType::STATIC);
	pbody->ctype = ColliderType::POTION;
	pbody->listener = this;

	return true;
}

bool Potion::Update(float dt)
{
	//Get the size of the window
	app->win->GetWindowSize(windowW, windowH);

	app->input->GetMousePosition(mouseX, mouseY); 
	mouseX *= app->win->GetScale();
	mouseY *= app->win->GetScale();

	Player* player;
	player = app->entityManager->GetPlayerEntity();

	// L07 DONE 4: Add a physics to an food - update the position of the object from the physics.  
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 8;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 8;

	posX = (int)windowW / 2 - 350 + position.x;
	posY = (int)windowH / 2 - 350 + position.y;

	if (app->input->GetKey(SDL_SCANCODE_TAB) == KEY_REPEAT) {
		app->render->DrawTexture(texture1, (int)windowW / 2 - 350, (int)windowH / 2 - 350, 0, 0);
		//TODO: Cada pocion hace su cosa
		if (isCreated) {
			LOG("mouseX: %d, mouseY: %d", mouseX, mouseY);
			LOG("posX: %d, posY: %d", posX, posY);
			//TODO: Dibujar la textura de la pocion
			if (mouseX >= posX && mouseX <= posX + 100 && mouseY >= posY && mouseY <= posY + 100) {
				app->render->DrawTexture(texture2, posX, posY, 0, 0);
			}
			else {
				app->render->DrawTexture(texture, posX, posY, 0, 0);
			}

		}
	}

	return true;        
}

bool Potion::CleanUp()
{
	app->physics->DestroyBody(pbody);
	return true;
}

void Potion::OnCollision(PhysBody* physA, PhysBody* physB) {

}

void Potion::EndCollision(PhysBody* physA, PhysBody* physB)
{
	
}