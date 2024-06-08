#ifndef __UI_H__
#define __UI_H__

#include "Core/Physics.h"

#include <box2d/b2_fixture.h>
#include <SDL.h>
#include <box2d/b2_math.h>
#include "Core/GuiControlButton.h"

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

	// OnGuiMouseClickEvent
	bool OnGuiMouseClickEvent(GuiControl* control) override;

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

	// Textures for the bestiary
	SDL_Texture* bestiaryPage1Texture = nullptr; // texture for page 1
	SDL_Texture* bestiaryPage2Texture = nullptr; // texture for page 2
	SDL_Texture* bestiaryPage3Texture = nullptr; // texture for page 3
	SDL_Texture* bestiaryPage4Texture = nullptr; // texture for page 4
	SDL_Texture* bestiaryPage5Texture = nullptr; // texture for page 5

	GuiControlButton* buttonPag1 = nullptr;
	GuiControlButton* buttonPag2 = nullptr;
	GuiControlButton* buttonPag3 = nullptr;
	GuiControlButton* buttonPag4 = nullptr;
	GuiControlButton* buttonPag5 = nullptr;

	uint windowW, windowH;

	bool openBestiary = false;
};

#endif // __UI_H__