#ifndef __UI_H__
#define __UI_H__

#include "Core/Physics.h"

#include <box2d/b2_fixture.h>
#include <SDL.h>
#include <box2d/b2_math.h>

struct SDL_Texture;
class GuiControlButton;

class UI : public Module
{
public:

	UI();

	UI(bool startEnabled);

	virtual ~UI();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:

	//Potions Inventory
	SDL_Texture* texture_inventory = NULL;
	SDL_Texture* celeritaPotionCreated;
	SDL_Texture* celeritaPotionSelected;
	SDL_Texture* etherPotionCreated;
	SDL_Texture* etherPotionSelected;
	SDL_Texture* oblitiusPotionCreated;
	SDL_Texture* oblitiusPotionSelected;
	SDL_Texture* VitaPotionCreated;
	SDL_Texture* VitaPotionSelected;

	SDL_Texture* MenuPequeno;
	SDL_Texture* celeritaPotion;
	SDL_Texture* etherPotion;
	SDL_Texture* oblitiusPotion;
	SDL_Texture* VitaPotion;
	
	SDL_Texture* VidaBestiario;
	SDL_Texture* Espada;
	SDL_Texture* Cetro;
	SDL_Texture* Seleccion;


	uint windowW, windowH;
};

#endif // __UI_H__