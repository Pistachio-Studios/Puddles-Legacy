#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Core/Physics.h"
#include "Utils/Timer.h"
#include "Utils/StateMachine.h"

#include <box2d/b2_fixture.h>
#include <SDL.h>
#include <box2d/b2_math.h>

#include <imgui.h>

struct SDL_Texture;

enum PlayerClass
{
	KNIGHT,
	WIZARD
};

enum ItemType
{
	// Plants
	ARNICA_PLANT,
	HAWTHORN_PLANT,
	HEPATICA_PLANT,
	WITCH_HAZEL_PLANT,
	COMFREY_PLANT,
	// Potions
	VITA_POTION,
	CELERITA_POTION,
	ETHER_POTION,
	PUDICITIA_POTION,
	OBLITIUS_POTION,
};

class Item {
public:
    ItemType name;
    int quantity;
    std::string description;

    Item(ItemType name, int quantity, std::string description) : name(name), quantity(quantity), description(description) {}
};

class Inventory {
public:
    std::vector<Item> items;

	Inventory() {
		// Plants
		items.push_back(Item(ARNICA_PLANT, 0, "Permite craftear la poción de cura"));
		items.push_back(Item(HAWTHORN_PLANT, 0, "Permite craftear la poción de aumento de velocidad"));
		items.push_back(Item(HEPATICA_PLANT, 0, "Permite craftear la poción de recuperación de energía"));
		items.push_back(Item(WITCH_HAZEL_PLANT, 0, "Permite craftear la poción de quitar efectos negativos"));
		items.push_back(Item(COMFREY_PLANT, 0, "Permite craftear la poción de resetear árbol de habilidades"));

		// Potions
		items.push_back(Item(VITA_POTION, 0, "Cura vida"));
		items.push_back(Item(CELERITA_POTION, 0, "Aumenta velocidad"));
		items.push_back(Item(ETHER_POTION, 0, "Recupera energia"));
		items.push_back(Item(PUDICITIA_POTION, 0, "Quita efectos negativos"));
		items.push_back(Item(OBLITIUS_POTION, 0, "Resetea arbol de habilidades"));
	}

    void AddItem(ItemType itemType) {
        for (auto& item : items) {
            if (item.name == itemType) {
                item.quantity++;
                break;
            }
        }
    }

	void RemoveItem(ItemType itemType) {
		for (auto& item : items) {
			if (item.name == itemType) {
				if (item.quantity > 0) {
					item.quantity--;
				}
				break;
			}
		}
	}

	Item* GetItem(ItemType itemName) {
		for (auto& item : items) {
			if (item.name == itemName) {
				return &item;
			}
		return nullptr;
		}
	}

	bool HasItem(ItemType itemName) {
		for (auto& item : items) {
			if (item.name == itemName) {
				return true;
			}
		}
		return false;
	}

	void DrawImGui() {
		ImGui::Begin("Inventory");
		for (auto& item : items) {
			ImGui::Text(item.name == ARNICA_PLANT ? "Arnica Plant" :
				item.name == HAWTHORN_PLANT ? "Hawthorn Plant" :
				item.name == HEPATICA_PLANT ? "Hepatica Plant" :
				item.name == WITCH_HAZEL_PLANT ? "Witch Hazel Plant" :
				item.name == COMFREY_PLANT ? "Comfrey Plant" :
				item.name == VITA_POTION ? "Vita Potion" :
				item.name == CELERITA_POTION ? "Celerita Potion" :
				item.name == ETHER_POTION ? "Ether Potion" :
				item.name == PUDICITIA_POTION ? "Pudicitia Potion" :
				item.name == OBLITIUS_POTION ? "Oblitius Potion" : "");
			ImGui::Text(("Quantity: " + std::to_string(item.quantity)).c_str());
			ImGui::Text(item.description.c_str());

			if (ImGui::Button(("Add " + std::string(item.name == ARNICA_PLANT ? "Arnica Plant" :
				item.name == HAWTHORN_PLANT ? "Hawthorn Plant" :
				item.name == HEPATICA_PLANT ? "Hepatica Plant" :
				item.name == WITCH_HAZEL_PLANT ? "Witch Hazel Plant" :
				item.name == COMFREY_PLANT ? "Comfrey Plant" :
				item.name == VITA_POTION ? "Vita Potion" :
				item.name == CELERITA_POTION ? "Celerita Potion" :
				item.name == ETHER_POTION ? "Ether Potion" :
				item.name == PUDICITIA_POTION ? "Pudicitia Potion" :
				item.name == OBLITIUS_POTION ? "Oblitius Potion" : "")).c_str())) {
				AddItem(item.name);
			}
			ImGui::SameLine();
			if (ImGui::Button(("Remove " + std::string(item.name == ARNICA_PLANT ? "Arnica Plant" :
				item.name == HAWTHORN_PLANT ? "Hawthorn Plant" :
				item.name == HEPATICA_PLANT ? "Hepatica Plant" :
				item.name == WITCH_HAZEL_PLANT ? "Witch Hazel Plant" :
				item.name == COMFREY_PLANT ? "Comfrey Plant" :
				item.name == VITA_POTION ? "Vita Potion" :
				item.name == CELERITA_POTION ? "Celerita Potion" :
				item.name == ETHER_POTION ? "Ether Potion" :
				item.name == PUDICITIA_POTION ? "Pudicitia Potion" :
				item.name == OBLITIUS_POTION ? "Oblitius Potion" : "")).c_str())) {
				RemoveItem(item.name);
			}
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

public:

	const char* texturePath;
	SDL_Texture* texture = NULL;

	float vida = 10.0f;
	float dano = 4.0f;

	//Movement
	float moveForce = 1.0f;
	float maxSpeed = 5.0f;

	//State Machines
	StateMachine<Player>* movementFSM = nullptr;
	StateMachine<Player>* combatFSM = nullptr;

	PhysBody* pbody;

	b2Vec2 lookingDir;
	float lookingAngle = 0;

	PlayerClass currentClass = KNIGHT;

	Inventory inventory;

	//tmps
	PhysBody* sword;

	int livesPlayer = 10;
	int totalLivesPlayer;
	bool deadPlayer = false;

	bool sceneChange = false;
};

#endif // __PLAYER_H__