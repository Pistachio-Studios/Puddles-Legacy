#include "Core/EntityManager.h"
#include "Gameplay/Entities/Player.h"
#include "Gameplay/Entities/EnemyBoss.h"
#include "Core/App.h"
#include "Core/Textures.h"
#include "Gameplay/Scene.h"
#include "Core/DebugUI.h"

#include "Utils/Defs.h"
#include "Utils/Log.h"
#include <tracy/Tracy.hpp>

#include <imgui.h>

EntityManager::EntityManager() : Module()
{
	name.Create("entitymanager");
}

EntityManager::EntityManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type)
{
	Entity* entity = nullptr;

	switch (type)
	{
	case EntityType::PLAYER:
		entity = new Player();
		break;
	case EntityType::ENEMYBOSS:
		entity = new EnemyBoss();
		break;
	default:
		break;
	}

	entities.Add(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item = entities.start;
	ListItem<Entity*>* nextItem = nullptr;

	while (item != nullptr)
	{
		nextItem = item->next; // Store the next item before possibly deleting current item

		if (item->data == entity)
		{
			item->data->CleanUp();
			entities.Del(item);
			break; // Exit the loop after deleting the entity
		}

		item = nextItem; // Move to the next item
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

bool EntityManager::Update(float dt)
{
	// OPTICK PROFILIN
	ZoneScoped;

	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Update(dt);
		if(pEntity->entityDebugDraw) pEntity->DrawImGui();
	}

	return ret;
}

void EntityManager::DrawImGui()
{
	if(app->debugUI->entityManagerSpawnEntity)
	{
		ImGui::Begin("Entity Manager");
		
		static EntityType selectedEntityType = EntityType::PLAYER;
		
		const char* entityTypes[] = { "PLAYER", "ENEMYBOSS"};

		ImGui::Combo("Entity Type", (int*)&selectedEntityType, entityTypes, IM_ARRAYSIZE(entityTypes));
		if (ImGui::Button("Spawn Entity"))
		{
			EntityType type = static_cast<EntityType>(selectedEntityType);

			Entity* entity;

			entity = CreateEntity(type);
			entity->Start();
		}
		
		ImGui::End();
	}

	if(app->debugUI->entityManagerEntityList)
	{
		ListItem<Entity*>* item;
		Entity* pEntity = nullptr;
		
		ImGui::Begin("Entity List");
		for (item = entities.start; item != nullptr; item = item->next)
		{
			pEntity = item->data;
			if (ImGui::Selectable(pEntity->name.GetString(), pEntity->entityDebugDraw)) {
				pEntity->entityDebugDraw = !pEntity->entityDebugDraw;
			}
		}
		ImGui::End();
	}

	if(app->debugUI->entityManagerInfo)
	{
		ImGui::Begin("Entity Manager");
		ImGui::Text("Entities: %d", entities.Count());
		ImGui::End();
	
	}
}

bool EntityManager::SaveState(pugi::xml_node node) {
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		
		pEntity->SaveState(node);
	}

	return ret;
}

bool EntityManager::LoadState(pugi::xml_node node)
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;

		pEntity->LoadState(node);
	}

	return ret;
}