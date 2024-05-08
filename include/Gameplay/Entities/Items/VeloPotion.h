//#ifndef __VELOPOTION_H__
//#define __VELOPOTION_H__
//
//#include "Core/Physics.h"
//#include "Utils/Timer.h"
//#include "Utils/StateMachine.h"
//#include "Gameplay/Entities/Items/Item.h"
//
//#include <box2d/b2_fixture.h>
//#include <SDL.h>
//#include <box2d/b2_math.h>
//
//struct SDL_Texture;
//
//class VeloPotion : public Item
//{
//public:
//
//	bool startTimer = true;
//	Timer timer;
//
//	VeloPotion();
//
//	virtual ~VeloPotion();
//
//	bool Awake() override;
//
//	bool Start() override;
//
//	bool Update(float dt) override;
//
//	bool CleanUp() override;
//
//
//public:
//
//	const char* texturePath;
//	SDL_Texture* texture = NULL;
//
//	PhysBody* pbody;
//};
//
//#endif // __VELOPOTION_H__