#include "Core/EntityManager.h"
#include "Gameplay/Entities/Player.h"
#include "Gameplay/Entities/OwlEnemy.h"
#include "Gameplay/Entities/DogEnemy.h"
#include "Gameplay/Entities/FurBall.h"
#include "Core/App.h"
#include "Core/Textures.h"
#include "Gameplay/Scene.h"
#include "Gameplay/Entities/ScoreItem.h"
#include "Gameplay/Entities/FoodItem.h"
#include "Gameplay/Entities/Checkpoint.h"

#include "Utils/Defs.h"
#include "Utils/Log.h"

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
	case EntityType::OWLENEMY:
		entity = new OwlEnemy();
		break;
	case EntityType::DOGENEMY:
		entity = new DogEnemy();
		break;
	case EntityType::FURBALL:
		entity = new FurBall();
		break;
	case EntityType::SCOREITEM:
		entity = new ScoreItem();
		break;
	case EntityType::FOODITEM:
		entity = new FoodItem();
		break;
	case EntityType::CHECKPOINT:
		entity = new Checkpoint();
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
	////OPTICK_EVENT();

	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Update(dt);
	}

	return ret;
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