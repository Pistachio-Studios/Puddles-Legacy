//#include "Gameplay/Entities/Items/Item.h"
//#include "Gameplay/Entities/Items/Item/VeloPotion.h"
//#include "Core/App.h"
//#include "Gameplay/Entities/Entity.h"
//#include "Core/Textures.h"
//#include "Core/Input.h"
//#include "Core/AnimationManager.h"
//#include "Core/Render.h"
//#include "Gameplay/Scene.h"
//#include "Utils/Point.h"
//#include "Core/Physics.h"
//
//#include <cmath>
//#include <iostream>
//
//#include <box2d/b2_math.h>
//#include <box2d/b2_body.h>
//#include <box2d/b2_fixture.h>
//
//#include <imgui.h>
//
//VeloPotion::VeloPotion()
//{
//	name.Create("HealPotion");
//}
//
//VeloPotion::~VeloPotion() {
//
//}
//
//bool VeloPotion::Awake() {
//
//	return true;
//}
//
//bool VeloPotion::Start() {
//	Item::Start();
//
//	animItem = *app->animationManager->GetAnimByName("testerAnim");
//	animItem.speed = 2.0f;
//
//	timer = Timer();
//
//	return true;
//}
//
//bool VeloPotion::Update(float dt)
//{
//	Item::Update(dt);
//	return true;
//}
//
//
//bool VeloPotion::CleanUp() {
//	Item::CleanUp();
//	return true;
//}
