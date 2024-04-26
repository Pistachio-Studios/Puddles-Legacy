//#include "Core/AnimationManager.h"
//#include "Core/App.h"
//#include "Core/Render.h"
//#include "Utils/SString.h"
//#include "Core/Textures.h"
//#include "Core/Map.h"
//#include "Core/Physics.h"
//#include "Core/Window.h"
//#include "Gameplay/Scene.h"
//
//#include "Utils/Defs.h"
//#include "Utils/Log.h"
//
//#include <math.h>
//
//#include <SDL_image.h>
//#include <box2d/b2_fixture.h>
//#include <tracy/Tracy.hpp>
//
//
//AnimationManager::AnimationManager() : Module(), mapLoaded(false)
//{
//    name.Create("animations");
//}
//
//AnimationManager::AnimationManager(bool startEnabled) : Module(startEnabled), mapLoaded(false)
//{
//    name.Create("animations");
//}
//
//// Destructor
//AnimationManager::~AnimationManager()
//{}
//
//// Called before render is available
//bool AnimationManager::Awake(pugi::xml_node& config)
//{
//    LOG("Loading Animation Parser");
//    bool ret = true;
//
//    return ret;
//}
//
//bool AnimationManager::Start() {
//
//    //Calls the functon to load the map, make sure that the filename is assigned
//    SString animPath = path;
//    animPath += name;
//    bool ret = Load(animPath);
//
//    return ret;
//}
//
//bool Map::Update(float dt)
//{
//
//
//}
//
//Animation* AnimationManager::GetAnimByName(SString name)
//{
//    ListItem<Animation*>* item = mapData.animations.start;
//    Animation* set = NULL;
//
//    while (item)
//    {
//        set = item->data;
//        if (item->data->name == name)
//        {
//            return set;
//        }
//        item = item->next;
//    }
//
//    return set;
//}
//
//bool AnimationManager::CleanUp()
//{
//    LOG("Unloading map");
//
//    //Clean up pathfing class 
//    //if(pathfinding != nullptr)pathfinding->CleanUp(); //TODO Mirar porque necesita esta comprobaci?n, no deberia necesitarla.
//
//    ListItem<TileSet*>* tileset;
//    tileset = mapData.tilesets.start;
//
//    while (tileset != NULL) {
//        RELEASE(tileset->data);
//        tileset = tileset->next;
//    }
//
//    mapData.tilesets.Clear();
//
//    ListItem<MapLayer*>* layerItem;
//    layerItem = mapData.maplayers.start;
//
//    while (layerItem != NULL) {
//        RELEASE(layerItem->data->data);
//        RELEASE(layerItem->data);
//        layerItem = layerItem->next;
//    }
//
//    mapData.maplayers.Clear();
//
//    return true;
//}
//
//bool AnimationManager::LoadAnimation(pugi::xml_node node, TileSet* tileset)
//{
//    bool ret = true;
//
//    Animation* anim = new Animation();
//    anim->name = tileset->name;
//    anim->texture = tileset->texture;
//
//    for (pugi::xml_node frameNode = node.child("animation").child("frame"); frameNode && ret; frameNode = frameNode.next_sibling("frame"))
//    {
//        int id = frameNode.attribute("tileid").as_int();
//        int tilesPerRow = tileset->columns;
//        int x = (id % tilesPerRow) * tileset->tileWidth;
//        int y = (id / tilesPerRow) * tileset->tileHeight;
//        anim->PushBack({ x, y, tileset->tileWidth, tileset->tileHeight });
//    }
//
//    mapData.animations.Add(anim);
//
//    return ret;
//}
