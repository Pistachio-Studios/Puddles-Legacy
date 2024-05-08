//#include "gameplay/entities/items/item.h"
//
//#include "core/audio.h"
//
//#include "utils/log.h"
//
//#include "core/app.h"
//#include "gameplay/entities/entity.h"
//#include "gameplay/entities/player.h"
//#include "core/textures.h"
//#include "core/input.h"
//#include "core/render.h"
//#include "gameplay/scene.h"
//#include "utils/point.h"
//#include "core/physics.h"
//#include "utils/statemachine.h"
//#include "core/scenemanager.h"
//#include "core/map.h"
//
//#include <cstdlib>
//
//#ifdef __linux__
//#include <box2d/dynamics/b2body.h>
//#endif
//
//item::item() : entity(entitytype::item)
//{
//	name.create("item");
//}
//
//item::~item() {}
//
//bool item::awake() {
//
//	position.x = parameters.attribute("x").as_int();
//	position.y = parameters.attribute("y").as_int();
//	
//	texture = app->tex->load("assets/textures/click.png");
//
//	pbody = app->physics->createrectangle(position.x, position.y, 42, 65, bodytype::static);
//	pbody->listener = this;
//	pbody->ctype = collidertype::item;
//
//	return true;
//}
//
//bool item::start() {
//
//	/*// pick a random texture
//	int randomindex = rand() % foodtextures.totalframes;
//	foodtextures.currentframe = randomindex;
//	foodtextures.loop = true;*/
//
//	return true;
//}
//
//bool item::update(float dt)
//{
//	mouseworldposition = { pixel_to_meters(app->input->getmousex()) + pixel_to_meters(-app->render->camera.x), pixel_to_meters(app->input->getmousey()) + pixel_to_meters(-app->render->camera.y) };
//	pbody->body->settransform(pbody->body->getposition(), 0);
//
//	int width, height;
//	sdl_querytexture(animitem.texture, null, null, &width, &height);
//
//	//update player position in pixels
//	position.x = meters_to_pixels(pbody->body->gettransform().p.x) - width / 2;
//	position.y = meters_to_pixels(pbody->body->gettransform().p.y) - height / 2;
//
//	if (mousex > position.x && mousex < position.x + width && mousey > position.y && mousey < position.y + height) {
//		app->render->drawtexture(texture, position.x - 40, position.y - 50);
//	}
//
//	app->render->drawtexture(animitem.texture, position.x, position.y, &anim.getcurrentframe());
//	animitem.update(dt);
//
//
//	return true;
//}
//
//bool item::cleanup()
//{
//	app->physics->destroybody(pbody);
//	return true;
//}
//
//void item::oncollision(physbody* physa, physbody* physb)
//{
//	if (physb->ctype == collidertype::player) {
//		if (app->input->getmousebuttondown(sdl_button_left) == key_down) {
//			ispicked = true;
//			app->entitymanager->destroyentity(this);
//		}
//	}
//}