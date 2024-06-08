#ifndef __PLAYERPOINTANDCLICK_H__
#define __PLAYERPOINTANDCLICK_H__

#include "Gameplay/Entities/Entity.h"

class ParticleGenerator;
class SDL_Texture;

class PlayerPointAndClick : public Entity
{
public:

	PlayerPointAndClick();
	
	virtual ~PlayerPointAndClick();

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

	PhysBody* pbody;
	int margin = 50;

	ParticleGenerator* effects = nullptr;

	SDL_Texture* mouseNoSelection = nullptr;
	SDL_Texture* mouseSelection = nullptr;

	bool hovering = false;
};

#endif // __PLAYERPOINTANDCLICK_H__