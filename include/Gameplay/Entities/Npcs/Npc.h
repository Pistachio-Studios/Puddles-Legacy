#ifndef __NPCS_H__
#define __NPCS_H__

#include "Core/Physics.h"
#include "Utils/Timer.h"


struct SDL_Texture;

class Npc : public Entity
{
public:

	bool startTimer = true;
	Timer timer;

	Npc();

	virtual ~Npc();

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

	const char* path;
	SDL_Texture* texture2 = NULL;

	const char* path2;
	SDL_Texture* texture3 = NULL;

	bool touchingNpc = false; 

	PhysBody* pbody;
	PhysBody* npcSensor;
	bool npcClick = false;

	b2Vec2 mouseWorldPosition;

private:
	int mouseX, mouseY;
};

#endif // __NPCS_H__