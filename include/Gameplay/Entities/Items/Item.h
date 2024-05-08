//#ifndef __ITEM_H__
//#define __ITEM_H__
//
//#include "Core/Animation.h"
//#include "Core/Physics.h"
//#include "Utils/Timer.h"
//#include "Utils/Point.h"
//#ifdef __linux__
//#include <SDL.h>
//#elif _MSC_VER
//#include "SDL.h"
//#endif
//
//struct SDL_Texture;
//class Player;
//
//class Item : public Entity
//{
//public:
//
//	Item();
//	virtual ~Item();
//
//	bool Awake() override;
//
//	bool Start() override;
//
//	bool Update(float dt) override;
//
//	bool CleanUp() override;
//
//	void OnCollision(PhysBody* physA, PhysBody* physB) override;
//
//public:
//
//	bool isPicked = false;
//	SDL_Texture* texture;
//	const char* texturePath;
//	Player* player;
//
//private:
//	PhysBody* pbody;
//};
//
//#endif // __ITEM_H__