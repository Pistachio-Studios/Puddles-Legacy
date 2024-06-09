#ifndef __TAVERNSCENE_H__
#define __TAVERNSCENE_H__

#include "Core/GuiControlLabel.h"
#include "Core/Module.h"
#include "Gameplay/Entities/Player.h"
#include "Gameplay/Entities/PlayerPointAndClick.h"
#include "Core/GuiControl.h"
#include "Core/GuiControlButton.h"
#include "Core/GuiControlPopUp.h"
#include "Gameplay/Scene.h"

struct SDL_Texture;

class TavernScene : public Scene
{
public:

	// Constructor
	TavernScene(const SString& name) : Scene(name) {}

	// Destructor
	virtual ~TavernScene();

	// Called before the first frame
	bool Enter() override;

	// Called before all Updates
	bool PreUpdate() override;

	// Called each loop iteration
	bool Update(float dt) override;

	// Called before all Updates
	bool PostUpdate() override;

	bool Exit() override;

	// Called before quitting
	bool CleanUp() override;

	// Handles multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control) override;

	void RenderGUI();

	void ResetPotionPopUps();
	void ManagePotionCreation(Inventory* playerInventory, SDL_Texture* craftableTex, SDL_Texture* notCraftableTex, GuiControlPopUp*& craftablePopup, GuiControlPopUp*& notCraftablePopup);
	bool CheckIngredient(Inventory* playerInventory, const std::string& ingredientName, int requiredQuantity);
public:
	bool winCondition = false;
	Player* player;

	PlayerPointAndClick* playerPointAndClick;
	PhysBody* cauldronTrigger; 

private:
	SDL_Texture* img;
	SDL_Texture* cauldronTex;
	SDL_Texture* cauldronSelectTex;
	SDL_Texture* CeleritaPotionTex;
	SDL_Texture* VitaPotionTex;
	SDL_Texture* EtherPotionTex;
	SDL_Texture* OblitiusPotionTex;

	SDL_Texture* NotCeleritaPotionTex;
	SDL_Texture* NotVitaPotionTex;
	SDL_Texture* NotEtherPotionTex;
	SDL_Texture* NotOblitiusPotionTex;

	SDL_Texture* currentPotion;

	SDL_Texture* prueba;
	float textPosX, textPosY = 0;
	uint texW, texH;
	uint windowW, windowH;
	GuiControlButton* gcButton;
	GuiControlLabel* gcScore;
	GuiControlLabel* gcLives;
	GuiControlLabel* gcTime;

	GuiControlButton* gcResume;
	GuiControlButton* gcSave;
	GuiControlButton* gcSettings;
	GuiControlButton* gcBackToTitle;
	GuiControlButton* gcExit;

	GuiControlButton* cauldronSelectExit;
	GuiControlButton* cauldronExit;
	GuiControlButton* cauldronCreate;
	GuiControlButton* potionCreateButton;
	GuiControlPopUp* cauldron = nullptr;
	GuiControlPopUp* cauldronSelect = nullptr;

	GuiControlPopUp* CrafteableCeleritaPotion = nullptr;
	GuiControlPopUp* CrafteableVitaPotion = nullptr;
	GuiControlPopUp* CrafteableEtherPotion = nullptr;
	GuiControlPopUp* CrafteableOblitiusPotion = nullptr;

	GuiControlPopUp* NotCrafteableCeleritaPotion = nullptr;
	GuiControlPopUp* NotCrafteableVitaPotion = nullptr;
	GuiControlPopUp* NotCrafteableEtherPotion = nullptr;
	GuiControlPopUp* NotCrafteableOblitiusPotion = nullptr;

	bool paused = false;
	bool exitPressed = false;
	bool selectExitPressed = false;
	bool cauldronExitPressed = false;
	bool cauldronCreatePressed = false;
	bool potionCreatePressed = false;

	int type = 1;

	Timer* playingTime;
};

#endif // __TAVERNSCENE_H__