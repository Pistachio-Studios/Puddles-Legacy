#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Core/Physics.h"
#include "Utils/Timer.h"
#include "Utils/StateMachine.h"
#include "Gameplay/Entities/Items/Item.h"
#include "Gameplay/Entities/Items/ArnicaPlant.h"
#include "Gameplay/Entities/Items/HepaticaPlant.h"
#include "Gameplay/Entities/Items/ComfreyPlant.h"
#include "Gameplay/Entities/Items/WitchHazelPlant.h"
#include "Gameplay/Entities/Items/HawthornPlant.h"
#include "Gameplay/Entities/Items/VitaPotion.h"
#include "Gameplay/Entities/Items/CeleritaPotion.h"
#include "Gameplay/Entities/Items/EtherPotion.h"
#include "Gameplay/Entities/Items/OblitiusPotion.h"
#include "Core/UI.h"
#include "Core/GuiControlPopUp.h"
#include "Core/GuiManager.h"
#include "Core/GuiControl.h"
#include "Core/App.h"

#include <box2d/b2_fixture.h>
#include <SDL.h>
#include <box2d/b2_math.h>

#include <imgui.h>

struct SDL_Texture;
class Sword;
class Staff;
class ParticleGenerator;
class Light;

enum PlayerClass
{
	KNIGHT,
	WIZARD
};

class Bestiary : public Scene{
public:

	int currentPage = 1; // The current page number
	const int totalPages = 5; // The total number of pages

	// Pag 1 attributes
	bool mission1Completed = false;
	bool mission2Completed = false;

	bool swordAbility100Unlocked = false;
	bool swordAbility110Unlocked = false;
	bool swordAbility111Unlocked = false;
	bool swordAbility112Unlocked = false;
	bool swordAbility120Unlocked = false;
	bool swordAbility122Unlocked = false;
	bool swordAbility123Unlocked = false;

	bool staffAbility100Unlocked = false;
	bool staffAbility110Unlocked = false;
	bool staffAbility111Unlocked = false;
	bool staffAbility112Unlocked = false;
	bool staffAbility120Unlocked = false;
	bool staffAbility122Unlocked = false;
	bool staffAbility123Unlocked = false;

	// Pag 2 attributes
	bool enemy1Killed = false;
	bool enemy2Killed = false;
	bool enemy3Killed = false;

	// Pag 3 attributes
	bool hepaticaPlantCollected = false;
	bool comfreyPlantCollected = false;
	bool hawthornPlantCollected = false;
	bool witchhazelPlantCollected = false;
	bool arnicaPlantCollected = false;

	// Pag 4 attributes
	bool klausUnlocked = false;
	bool bountyUnlocked = false;

	// Pag 5 attributes
	// Nothing cause there are no draws 💀

	// Mission Booleans
	bool forestUnlocked = false;
	bool cauldronUnlocked = false;
	bool changedClassUnlocked = false; // TODO change to true when the player changes class

public:

	void nextPage() {
		if (currentPage < totalPages) {
			currentPage++;
		}
	};

	void previousPage() {
		if (currentPage > 1) {
			currentPage--;
		}
	};

	void setPage(int page) {
		if (page >= 1 && page <= totalPages) {
			currentPage = page;
		}
	};
};

class Inventory {
public:
	std::vector<Item*> items;

	Inventory() {
		// Plants
		// TODO revisar descripciones - DONE
		items.push_back(new ArnicaPlant("Arnica Plant", 0, "Para craftear la poción de cura o la de aumentar velocidad"));
		items.push_back(new HepaticaPlant("Hepatica Plant", 0, "Para craftear la poción de resetear árbol de habilidades"));
		items.push_back(new ComfreyPlant("Comfrey Plant", 0, "Para craftear la poción de resetear árbol de habilidades o la de recuperar energia"));
		items.push_back(new WitchHazelPlant("Witch Hazel Plant", 0, "Para craftear la poción de aumentar velocidad")); 
		items.push_back(new HawthornPlant("Hawthorn Plant", 0, "Para craftear la poción de cura o la de recuperar energia"));

		// Potions
		// TODO revisar descripciones - DONE
		items.push_back(new VitaPotion("Vita Potion", 0, "Cura vida"));
		items.push_back(new CeleritaPotion("Celerita Potion", 0, "Aumenta velocidad"));
		items.push_back(new EtherPotion("Ether Potion", 0, "Recupera energia"));
		items.push_back(new OblitiusPotion("Oblitius Potion", 0, "Resetea arbol de habilidades"));
	}

	~Inventory() {
		for (auto& item : items) {
			delete item;
		}
	}

	void AddNItems(std::string itemName, int n) {
		for (auto& item : items) {
			if (item->name == itemName) {
				item->quantity += n;
				break;
			}
		}
	}

	void AddItem(std::string itemName) {
		for (auto& item : items) {
			if (item->name == itemName) {
				item->quantity++;
				break;
			}
		}
	}

	void RemoveItem(std::string itemName) {
		for (auto& item : items) {
			if (item->name == itemName) {
				if (item->quantity > 0) {
					item->quantity--;
				}
				break;
			}
		}
	}

	Item* GetItem(std::string itemName) {
		for (auto& item : items) {
			if (item->name == itemName) {
				return item;
			}
		}
		return nullptr;
	}

	bool HasItem(std::string itemName) {
		for (auto& item : items) {
			if (item->name == itemName) {
				return true;
			}
		}
		return false;
	}

	void Update(float dt) {
		for (auto& item : items) {
			if(item->GetType() == EntityType::POTION) {
				Potion* potion = static_cast<Potion*>(item);
				potion->Update(dt);
			}
		}
	}

	void DrawImGui() {
		ImGui::Begin("Inventory");
		for (auto& item : items) {
			ImGui::Text(item->name.c_str());
			ImGui::Text(("Quantity: " + std::to_string(item->quantity)).c_str());
			ImGui::Text(item->description.c_str());

			if (ImGui::Button(("Add " + item->name).c_str())) {
				AddItem(item->name);
			};
			
			ImGui::SameLine();
			
			if (ImGui::Button(("Remove " + item->name).c_str())) {
				RemoveItem(item->name);
			};

			ImGui::Text(" ");
		}
		ImGui::End();
	}

};


class Player : public Entity
{
public:

	bool startTimer = true;
	Timer timer;

	Player();
	
	virtual ~Player();

	bool Awake() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

	void DrawImGui() override;

	void OnCollision(PhysBody* physA, PhysBody* physB) override;

	void EndCollision(PhysBody* physA, PhysBody* physB) override;

	void OnRaycastHit(b2Fixture* fixture, const b2Vec2& point,
					   const b2Vec2& normal, float32 fraction) override;

	bool SaveState(pugi::xml_node& node) override;
	bool LoadState(pugi::xml_node& node) override;

	void AbilitySword100();
	void AbilitySword110();
	void AbilitySword111();
	void AbilitySword112();
	void AbilitySword120();
	void AbilitySword122();
	void AbilitySword123();

	void AbilityStaff100();
	void AbilityStaff110();
	void AbilityStaff111();
	void AbilityStaff112();
	void AbilityStaff120();
	void AbilityStaff122();
	void AbilityStaff123();

public:

	Sword* swordEntity = nullptr;
	Staff* staffEntity = nullptr;

	const char* texturePath;
	SDL_Texture* texture = NULL;
	SDL_Texture* texture1 = NULL;

	float vida = 15.0f;
	float maxVida = 15.0f;
	float strength = 12.0f;
	float intelligence = 14.0f;
	float defense;

	int currentExperience = 0;
	int experienceToNextLevel;
	int level = 1;
	int levelCap = 6;
	int abilityPoints = 0;

	bool bleed = false;
	int bleedChance = 15; // 15% chance of bleeding

	bool paralysis = false;
	int paralysisChance = 10; // 10% chance of paralysis

	bool stealLife = false;
	float stealLifeRatio = strength * 0.1; // 10% of the damage dealt

	int abylityPoints = 0;

	iPoint shadowPosition = { 0,0 };

	//Movement
	float moveForce = 1.5f;
	float maxSpeed = 6.0f;

	//State Machines
	StateMachine<Player>* movementFSM = nullptr;
	StateMachine<Player>* combatFSM = nullptr;

	PhysBody* pbody;

	b2Vec2 lookingDir;
	float lookingAngle = 0;

	PlayerClass currentClass;

	Inventory inventory;
	Bestiary* bestiary = nullptr;
	Potion* currentPotion = nullptr;

	//tmps
	PhysBody* sword;

	//soundFX
	int footstep1Fx, footstep2Fx, footstep3Fx, footstep4Fx, swordSlashFx, firestaffFx, deathSabrinaFx, damagedSabrinaFx, blockFx, potionFx, dashFx;

	int footstepFx[4];
	int currentFootstepFx;

	ParticleGenerator* damage = nullptr;

	float mana = 100.0f;
	float manaRegeneration = 2.0f; // 2 mana per second

	bool deadPlayer = false;

	bool sceneChange = false;

	SDL_RendererFlip flip = SDL_FLIP_NONE;

	Timer playerHurtCultdown;
	Timer dashTimer;
	float dashCultdown = 2.0f;

	// Steps sound timer
	float timerSteps = 0.0f;

	// Sword sound timer
	float timerSword = 0.0f;

	float time = 300.0f;


	Animation SabrinaEspadaIdle, SabrinaEspadaMovDelante, SabrinaEspadaMovDerecha, SabrinaEspadaMovIzquierda, SabrinaEspadaMovDetras,
		SabrinaEspadaDano, SabrinaEspadaDash, SabrinaEspadaMuerte, SabrinaEspadaRecolectar,
		SabrinaEspadaAtaque, SabrinaCetroIdle, SabrinaCetroMovDelante, SabrinaCetroMovDerecha, SabrinaCetroMovIzquierda,
		SabrinaCetroMovDetras, SabrinaCetroDano, SabrinaCetroDash, SabrinaCetroMuerte, SabrinaCetroRecolectar, SabrinaCetroAtaque;
	Animation* currentAnim;

	//Cheats
	bool godMode = false;
	bool ghostMode = false;

	Light* playerLight = nullptr;

	bool winCondition = false;
};

#endif // __PLAYER_H__