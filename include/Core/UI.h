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

	void PauseMenuVisibility(bool visible);

public:

	// Potions Inventory
	SDL_Texture* texture_inventory = nullptr;
	SDL_Texture* celeritaPotionCreated = nullptr;
	SDL_Texture* celeritaPotionSelected = nullptr;
	SDL_Texture* etherPotionCreated = nullptr;
	SDL_Texture* etherPotionSelected = nullptr;
	SDL_Texture* oblitiusPotionCreated = nullptr;
	SDL_Texture* oblitiusPotionSelected = nullptr;
	SDL_Texture* VitaPotionCreated = nullptr;
	SDL_Texture* VitaPotionSelected = nullptr;

	SDL_Texture* MenuPequeno = nullptr;
	SDL_Texture* celeritaPotion = nullptr;
	SDL_Texture* etherPotion = nullptr;
	SDL_Texture* oblitiusPotion = nullptr;
	SDL_Texture* VitaPotion = nullptr;
	
	SDL_Texture* VidaBestiario = nullptr;
	SDL_Texture* Espada = nullptr;
	SDL_Texture* Cetro = nullptr;
	SDL_Texture* Seleccion = nullptr;

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

	// Textures for sword abilities
	SDL_Texture* swordAbility100Texture = nullptr;
	SDL_Texture* swordAbility110Texture = nullptr;
	SDL_Texture* swordAbility111Texture = nullptr;
	SDL_Texture* swordAbility112Texture = nullptr;
	SDL_Texture* swordAbility120Texture = nullptr;
	SDL_Texture* swordAbility122Texture = nullptr;
	SDL_Texture* swordAbility123Texture = nullptr;

	// Textures for staff abilities
	SDL_Texture* staffAbility100Texture = nullptr;
	SDL_Texture* staffAbility110Texture = nullptr;
	SDL_Texture* staffAbility111Texture = nullptr;
	SDL_Texture* staffAbility112Texture = nullptr;
	SDL_Texture* staffAbility120Texture = nullptr;
	SDL_Texture* staffAbility122Texture = nullptr;
	SDL_Texture* staffAbility123Texture = nullptr;

	// Button for the abilities
	GuiControlButton* buttonSwordAbility100 = nullptr;
	GuiControlButton* buttonSwordAbility110 = nullptr;
	GuiControlButton* buttonSwordAbility111 = nullptr;
	GuiControlButton* buttonSwordAbility112 = nullptr;
	GuiControlButton* buttonSwordAbility120 = nullptr;
	GuiControlButton* buttonSwordAbility122 = nullptr;
	GuiControlButton* buttonSwordAbility123 = nullptr;
	
	GuiControlButton* buttonStaffAbility100 = nullptr;
	GuiControlButton* buttonStaffAbility110 = nullptr;
	GuiControlButton* buttonStaffAbility111 = nullptr;
	GuiControlButton* buttonStaffAbility112 = nullptr;
	GuiControlButton* buttonStaffAbility120 = nullptr;
	GuiControlButton* buttonStaffAbility122 = nullptr;
	GuiControlButton* buttonStaffAbility123 = nullptr;

	// Textures for the enemies
	SDL_Texture* enemy1KilledTexture = nullptr;
	SDL_Texture* enemy2KilledTexture = nullptr;
	SDL_Texture* enemy3KilledTexture = nullptr;

	// Textures for the plants
	SDL_Texture* hepaticaPlantTexture = nullptr;
	SDL_Texture* comfreyPlantTexture = nullptr;
	SDL_Texture* hawthornPlantTexture = nullptr;
	SDL_Texture* witchhazelPlantTexture = nullptr;
	SDL_Texture* arnicaPlantTexture = nullptr;

	// Textures for the npcs
	SDL_Texture* klausUnlockedTexture = nullptr;
	SDL_Texture* bountyUnlockedTexture = nullptr;

	GuiControlButton* gcResume;
	GuiControlButton* gcSave;
	GuiControlButton* gcSettings;
	GuiControlButton* gcBackToTitle;
	GuiControlButton* gcExit;

	uint windowW, windowH;

	bool openBestiary = false;
	bool exitPressed = false;
	bool mainMenuPressed = false;
};

#endif // __UI_H__