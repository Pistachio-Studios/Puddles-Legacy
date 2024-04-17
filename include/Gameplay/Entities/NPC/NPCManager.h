#ifndef __NPCMANAGER_H__
#define __NPCMANAGER_H__

#include "Core/Physics.h"
#include "Utils/Timer.h"


struct SDL_Texture;

class NPCManager : public Entity
{
public:

	bool startTimer = true;
	Timer timer;

	NPCManager();

	virtual ~NPCManager();

	bool Awake() override;

	bool Start() override;

	bool Update(float dt) override;

	bool CleanUp() override;

	void OnCollision(PhysBody* physA, PhysBody* physB) override;

	void EndCollision(PhysBody* physA, PhysBody* physB) override;

	bool SaveState(pugi::xml_node& node) override;
	bool LoadState(pugi::xml_node& node) override;

public:

	const char* texturePath;
	SDL_Texture* texture = NULL;


	PhysBody* pbody;

};

#endif // __NPCMANAGER_H__