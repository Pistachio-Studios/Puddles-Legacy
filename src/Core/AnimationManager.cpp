#include "Core/AnimationManager.h"
#include "Core/App.h"
#include "Core/Render.h"
#include "Utils/SString.h"
#include "Core/Textures.h"
#include "Core/Map.h"
#include "Core/Physics.h"
#include "Core/Window.h"
#include "Gameplay/Scene.h"

#include "Utils/Defs.h"
#include "Utils/Log.h"

#include <math.h>

#include <SDL_image.h>
#include <box2d/b2_fixture.h>
#include <tracy/Tracy.hpp>


AnimationManager::AnimationManager() : Module()
{
    name.Create("animation_manager");
}

AnimationManager::AnimationManager(bool startEnabled) : Module(startEnabled)
{
    name.Create("animation_manager");
}

// Destructor
AnimationManager::~AnimationManager()
{}

// Called before render is available
bool AnimationManager::Awake(pugi::xml_node& config)
{
    LOG("Loading Anim Parser");
    bool ret = true;

    path = config.attribute("path").as_string();
    name2 = config.attribute("name2").as_string();
    LOG("path: %s", path.GetString());

    return ret;
}

bool AnimationManager::Start() {

    //Calls the functon to load the animations, make sure that the filename is assigned
    SString animPath = path;
    animPath += name2;
    bool ret = Load(animPath); //Assets/Animations/Animation.tmx

    return ret;
}

bool AnimationManager::Update(float dt)
{
    return true;
}

Animation* AnimationManager::GetAnimByName(SString name)
{
    ListItem<Animation*>* item = animData.animations.start;
    Animation* set = NULL;

    while (item)
    {
        set = item->data;
        if (item->data->name == name)
        {
            return set;
        }
        item = item->next;
    }

    return set;
}

TileSet* AnimationManager::GetTilesetFromTileId(int gid) const
{
    ListItem<TileSet*>* item = animData.tilesets.start;
    TileSet* set = NULL;

    while (item)
    {
        set = item->data;
        if (gid < (item->data->firstgid + item->data->tilecount))
        {
            break;
        }
        item = item->next;
    }

    return set;
}

bool AnimationManager::CleanUp()
{
    LOG("Unloading animations");

    ListItem<Animation*>* animation;
    animation = animData.animations.start;

    while (animation != NULL) {
        app->tex->UnLoad(animation->data->texture);
        animation = animation->next;
    }

    animData.animations.Clear();

    return true;
}

bool AnimationManager::Load(SString animFileName)
{
    bool ret = true;

    pugi::xml_document animFileXML;
    pugi::xml_parse_result result = animFileXML.load_file(animFileName.GetString());
    LOG("animFileName: %s", animFileName.GetString());
    if (result == NULL)
    {
        LOG("Could not load animations xml file %s. pugi error: %s", animFileName.GetString(), result.description());
        ret = false;
    }

    if (ret == true)
    {
        ret = LoadTileSet(animFileXML);
    }

    if (ret == true)
    {
        LOG("Successfully parsed animations XML file :%s", animFileName.GetString());
        LOG("width : %d height : %d", animData.width, animData.height);
        LOG("tile_width : %d tile_height : %d", animData.tileWidth, animData.tileHeight);

        LOG("Tilesets----");

        ListItem<TileSet*>* tileset;
        tileset = animData.tilesets.start;

        while (tileset != NULL) {
            LOG("name : %s firstgid : %d", tileset->data->name.GetString(), tileset->data->firstgid);
            LOG("tile width : %d tile height : %d", tileset->data->tileWidth, tileset->data->tileHeight);
            LOG("spacing : %d margin : %d", tileset->data->spacing, tileset->data->margin);
            tileset = tileset->next;
        }
    }

    if (animFileXML) animFileXML.reset();

    return ret;
}

bool AnimationManager::LoadTileSet(pugi::xml_node animFile) {

    bool ret = true;

    pugi::xml_node tileset;
    for (tileset = animFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
    {
        TileSet* set = new TileSet();

        set->name = tileset.attribute("name").as_string();
        set->firstgid = tileset.attribute("firstgid").as_int();
        set->margin = tileset.attribute("margin").as_int();
        set->spacing = tileset.attribute("spacing").as_int();
        set->tileWidth = tileset.attribute("tilewidth").as_int();
        set->tileHeight = tileset.attribute("tileheight").as_int();     
        set->columns = tileset.attribute("columns").as_int();
        set->tilecount = tileset.attribute("tilecount").as_int();

        SString texPath = path;
        texPath += tileset.child("image").attribute("source").as_string();
        set->texture = app->tex->Load(texPath.GetString());

        if (tileset.child("tile").child("animation"))
        {
            LoadAnimation(tileset.child("tile"), set);
        }
    }

    return ret;
}

bool AnimationManager::LoadAnimation(pugi::xml_node node, TileSet* tileset)
{
    bool ret = true;

    Animation* anim = new Animation();
    anim->name = tileset->name;
    anim->texture = tileset->texture;

    for (pugi::xml_node frameNode = node.child("animation").child("frame"); frameNode && ret; frameNode = frameNode.next_sibling("frame"))
    {
        int id = frameNode.attribute("tileid").as_int();
        int duration = frameNode.attribute("duration").as_int();
        int tilesPerRow = tileset->columns;
        int x = (id % tilesPerRow) * tileset->tileWidth;
        int y = (id / tilesPerRow) * tileset->tileHeight;
        anim->PushBack({x, y, tileset->tileWidth, tileset->tileHeight}, duration);
    }

    animData.animations.Add(anim);

    return ret;
}

bool AnimationManager::LoadProperties(pugi::xml_node& node, Properties& properties)
{
    bool ret = false;

    for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
    {
        Properties::Property* p = new Properties::Property();
        p->name = propertieNode.attribute("name").as_string();
        p->value = propertieNode.attribute("value").as_bool(); // (!!) I'm assuming that all values are bool !!

        properties.list.Add(p);
    }

    return ret;
}

