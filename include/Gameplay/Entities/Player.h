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
	HEALING_PLANT,
	VELO_PLANT,
	ENERGY_PLANT,
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
		items.push_back(Item(HEALING_PLANT, 0, "Healing Plant Description..."));
		items.push_back(Item(VELO_PLANT, 0, "Velo Plant Description..."));
		items.push_back(Item(ENERGY_PLANT, 0, "Energy Plant Description..."));
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
		ImGui::Begin("Inventory", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
		for (auto& item : items) {
			ImGui::Text(item.name == HEALING_PLANT ? "Healing Plant" : item.name == VELO_PLANT ? "Velo Plant" : "Energy Plant");
			ImGui::Text(item.description.c_str());
			ImGui::Text(std::to_string(item.quantity).c_str());
			
			if (ImGui::Button(("Add " + std::string(item.name == HEALING_PLANT ? "Healing Plant" : item.name == VELO_PLANT ? "Velo Plant" : "Energy Plant")).c_str())) {
            	AddItem(item.name);
			}
			ImGui::SameLine();
			if (ImGui::Button(("Remove " + std::string(item.name == HEALING_PLANT ? "Healing Plant" : item.name == VELO_PLANT ? "Velo Plant" : "Energy Plant")).c_str())) {
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

	int healPlantCounter = 0;
	int energyPlantCounter = 0;
	int veloPlantCounter = 0;

	bool sceneChange = false;
};

#endif // __PLAYER_H__