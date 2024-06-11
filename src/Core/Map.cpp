#include "Core/Animation.h"
#include "Core/App.h"
#include "Core/Render.h"
#include "Utils/SString.h"
#include "Core/Textures.h"
#include "Core/Map.h"
#include "Core/Physics.h"
#include "Core/Window.h"
#include "Core/Lighting.h"
#include "Gameplay/Scene.h"

#include "Utils/Defs.h"
#include "Utils/Log.h"

#include <math.h>

#include <SDL_image.h>
#include <box2d/b2_fixture.h>
#include <tracy/Tracy.hpp>


Map::Map() : Module(), mapLoaded(false)
{
    name.Create("map");
}

Map::Map(bool startEnabled) : Module(startEnabled), mapLoaded(false)
{
    name.Create("map");
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    return ret;
}

bool Map::Start() {

    //Calls the functon to load the map, make sure that the filename is assigned
    SString mapPath = path;
    mapPath += name;
    bool ret = Load(mapPath);

    //Initialize the navigation map
    navigationMap = NULL;
    CreateNavigationMap(mapData.width, mapData.height, &navigationMap);

    //RELEASE_ARRAY(navigationMap);

    return ret;
}

bool Map::Update(float dt)
{
    // OPTICK PROFILIN
    ZoneScoped;

    if(mapLoaded == false)
        return false;

    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.maplayers.start;

    while (mapLayerItem != NULL) {

        if (mapLayerItem->data->properties.GetProperty("Draw") != NULL && std::get<bool>(mapLayerItem->data->properties.GetProperty("Draw")->value)) {

            iPoint cameraPosMap = WorldToMap((int)(-app->render->camera.x), (int)(-app->render->camera.y));
            //cameraPosMap = { 20,40 };

            int leftClipping = MAX(cameraPosMap.x, 0);
            int rightClipping = MIN(((cameraPosMap.x + (int)(app->render->camera.w / mapData.tileWidth) / app->win->GetScale()) + clippingMargin), mapLayerItem->data->width);
            
            int topClipping = MAX(cameraPosMap.y, 0);
            int bottomClipping = MIN(((cameraPosMap.y + (int)(app->render->camera.h / mapData.tileHeight) / app->win->GetScale()) + clippingMargin), mapLayerItem->data->height);


            // NO CLIPPING (DEBUG PURPOUSE)

            /*
            leftClipping = 0;
            rightClipping = mapLayerItem->data->width;
            topClipping = 0;
            bottomClipping = mapLayerItem->data->height;
            */
            
           int layer = 0;
           if(mapLayerItem->data->properties.GetProperty("layer") != NULL){
                auto valueVariant = mapLayerItem->data->properties.GetProperty("layer")->value;
                layer = std::get<int>(valueVariant);
           }
           

            for (int x = leftClipping; x < rightClipping; x++)
            {
                for (int y = topClipping; y < bottomClipping; y++)
                {
                    int gid = mapLayerItem->data->Get(x, y);
                    TileSet* tileset = GetTilesetFromTileId(gid);

                    SDL_Rect r = tileset->GetTileRect(gid);
                    iPoint pos = MapToWorld(x, y);

                    int pivotY = INT_MAX;
                    if(tileset->tilePivotY[gid] != NULL)
                    {
                        pivotY = tileset->tilePivotY[gid];
                    }


                    app->render->DrawTexture(tileset->texture,
                        pos.x,
                        pos.y,
                        &r,
                        1.0f,
                        0.0f,
                        1.0f,
                        layer,
                        SDL_FLIP_NONE,
                        INT_MAX,
                        pivotY);
                      
                        
                }
            }
        }
        mapLayerItem = mapLayerItem->next;

    }

    return true;
}

iPoint Map::MapToWorld(int x, int y) const
{
    iPoint ret;

    ret.x = x * mapData.tileWidth;
    ret.y = y * mapData.tileHeight;

    return ret;
}

iPoint Map::WorldToMap(int x, int y) 
{
    iPoint ret;

    ret.x = x / mapData.tileWidth;
    ret.y = y / mapData.tileHeight;

    return ret;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int gid) const
{
    SDL_Rect rect = { 0 };
    int relativeIndex = gid - firstgid;

    rect.w = tileWidth;
    rect.h = tileHeight;
    rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
    rect.y = margin + (tileWidth + spacing) * (relativeIndex / columns);

    return rect;
}

TileSet* Map::GetTilesetFromTileId(int gid) const
{
    ListItem<TileSet*>* item = mapData.tilesets.start;
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

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    //Clean up pathfing class 
    //if(pathfinding != nullptr)pathfinding->CleanUp(); //TODO Mirar porque necesita esta comprobaci�n, no deberia necesitarla.

    ListItem<TileSet*>* tileset;
    tileset = mapData.tilesets.start;

    while (tileset != NULL) {
        RELEASE(tileset->data);
        tileset = tileset->next;
    }

    mapData.tilesets.Clear();

    ListItem<MapLayer*>* layerItem;
    layerItem = mapData.maplayers.start;

    while (layerItem != NULL) {
        RELEASE(layerItem->data->data);
        RELEASE(layerItem->data);
        layerItem = layerItem->next;
    }

    mapData.maplayers.Clear();

    return true;
}

// Load new map
bool Map::Load(SString mapFileName)
{
    bool ret = true;

    pugi::xml_document mapFileXML;
    pugi::xml_parse_result result = mapFileXML.load_file(mapFileName.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", mapFileName.GetString(), result.description());
        ret = false;
    }

    if(ret == true)
    {
        ret = LoadMap(mapFileXML);
    }

    if (ret == true)
    {
        ret = LoadTileSet(mapFileXML);
    }

    if (ret == true)
    {
        ret = LoadAllLayers(mapFileXML.child("map"));
    }
    
    // NOTE: Later you have to create a function here to load and create the colliders from the map

    if (ret == true)
    {
        ret = LoadColliders(mapFileXML);
    }

    if (ret == true)
    {
        ret = LoadLights(mapFileXML);
    }
    
    if(ret == true)
    {
        LOG("Successfully parsed map XML file :%s", mapFileName.GetString());
        LOG("width : %d height : %d",mapData.width,mapData.height);
        LOG("tile_width : %d tile_height : %d",mapData.tileWidth, mapData.tileHeight);
        
        LOG("Tilesets----");

        ListItem<TileSet*>* tileset;
        tileset = mapData.tilesets.start;

        while (tileset != NULL) {
            LOG("name : %s firstgid : %d",tileset->data->name.GetString(), tileset->data->firstgid);
            LOG("tile width : %d tile height : %d", tileset->data->tileWidth, tileset->data->tileHeight);
            LOG("spacing : %d margin : %d", tileset->data->spacing, tileset->data->margin);
            tileset = tileset->next;
        }

        LOG("Layers----");

        ListItem<MapLayer*>* mapLayer;
        mapLayer = mapData.maplayers.start;

        while (mapLayer != NULL) {
            LOG("id : %d name : %s", mapLayer->data->id, mapLayer->data->name.GetString());
            LOG("Layer width : %d Layer height : %d", mapLayer->data->width, mapLayer->data->height);
            mapLayer = mapLayer->next;
        }
    }

    // Find the navigation layer
        ListItem<MapLayer*>* mapLayerItem;
        mapLayerItem = mapData.maplayers.start;
        navigationLayer = mapLayerItem->data;

        //Search the layer in the map that contains information for navigation
        while (mapLayerItem != NULL) {
            if (mapLayerItem->data->properties.GetProperty("Navigation") != NULL && std::get<bool>(mapLayerItem->data->properties.GetProperty("Navigation")->value)) {
                navigationLayer = mapLayerItem->data;
                break;
            }
            mapLayerItem = mapLayerItem->next;
        }

    if(mapFileXML) mapFileXML.reset();

    mapLoaded = ret;

    return ret;
}

bool Map::LoadMap(pugi::xml_node mapFile)
{
    bool ret = true;
    pugi::xml_node map = mapFile.child("map");

    if (map == NULL)
    {
        LOG("Error parsing map xml file: Cannot find 'map' tag.");
        ret = false;
    }
    else
    {
        //Load map general properties
        mapData.height = map.attribute("height").as_int();
        mapData.width = map.attribute("width").as_int();
        mapData.tileHeight = map.attribute("tileheight").as_int();
        mapData.tileWidth = map.attribute("tilewidth").as_int();
        mapData.type = MAPTYPE_UNKNOWN;
    }

    return ret;
}

bool Map::LoadTileSet(pugi::xml_node mapFile){

    bool ret = true; 

    pugi::xml_node tileset;
    for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
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

        {
            mapData.tilesets.Add(set);
        }
        pugi::xml_node tile;
        for (tile = tileset.child("tile"); tile && ret; tile = tile.next_sibling("tile"))
        {
            pugi::xml_node properties = tile.child("properties");
            if(properties)
            {
                pugi::xml_node property = properties.child("property");
                SString name = property.attribute("name").as_string();
                if(name == "pivotY")
                {
                    //set->tilePivotY.insert(std::pair<int, int>(set->firstgid + tile.attribute("id").as_int(), property.attribute("value").as_int()));
                    set->tilePivotY[set->firstgid + tile.attribute("id").as_int()] = property.attribute("value").as_int();
                }
            }
        }
    }

    return ret;
}

bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
    bool ret = true;

    //Load the attributes
    layer->id = node.attribute("id").as_int();
    layer->name = node.attribute("name").as_string();
    layer->width = node.attribute("width").as_int();
    layer->height = node.attribute("height").as_int();
    if (node.attribute("parallaxx"))
    {
        layer->parallaxFactor = node.attribute("parallaxx").as_float();
    }
    else
    {
        layer->parallaxFactor = 1.0f;
    }

    LoadProperties(node, layer->properties);

    //Reserve the memory for the data 
    layer->data = new uint[layer->width * layer->height];
    memset(layer->data, 0, layer->width * layer->height);

    //Iterate over all the tiles and assign the values
    pugi::xml_node tile;
    int i = 0;
    for (tile = node.child("data").child("tile"); tile && ret; tile = tile.next_sibling("tile"))
    {
        layer->data[i] = tile.attribute("gid").as_int();
        i++;
    }

    return ret;
}

bool Map::LoadAllLayers(pugi::xml_node mapNode) {
    bool ret = true;

    for (pugi::xml_node layerNode = mapNode.child("layer"); layerNode && ret; layerNode = layerNode.next_sibling("layer"))
    {
        //Load the layer
        MapLayer* mapLayer = new MapLayer();
        ret = LoadLayer(layerNode, mapLayer);

        //add the layer to the map
        mapData.maplayers.Add(mapLayer);
    }

    return ret;
}

bool Map::LoadColliders(pugi::xml_node mapFile)
{
    bool ret = true; 
    uint scale = app->win->GetScale();

    pugi::xml_node objectGroup;
    for (objectGroup = mapFile.child("map").child("objectgroup"); objectGroup && ret; objectGroup = objectGroup.next_sibling("objectgroup"))
    {
        pugi::xml_node properties = objectGroup.child("properties");
        pugi::xml_node property = properties.child("property");
        std::string objectType = property.attribute("value").as_string();

        if (objectType == "collider") {
            pugi::xml_node collider;
            for (collider = objectGroup.child("object"); collider && ret; collider = collider.next_sibling("object"))
            {

                if (SString(collider.attribute("type").as_string()) == "rectangle")
                {
                    Colliders* c = new Colliders();

                    c->x = collider.attribute("x").as_int();
                    c->y = collider.attribute("y").as_int();
                    c->width = collider.attribute("width").as_int();
                    c->height = collider.attribute("height").as_int();

                    PhysBody* c1 = app->physics->CreateRectangle(c->x + c->width / 2, c->y + c->height / 2, c->width, c->height, STATIC);

                    if (SString(objectGroup.attribute("class").as_string()) == "obstacle") {
                        c1->ctype = ColliderType::OBSTACLE;
                    }
                    else if (SString(objectGroup.attribute("class").as_string()) == "death")
                    {
                        c1->ctype = ColliderType::DEATH;
                        c1->body->GetFixtureList()->SetSensor(true);
                    }
                    else if (SString(objectGroup.attribute("class").as_string()) == "limits")
                    {
                        c1->ctype = ColliderType::LIMITS;
                    }
                    else if (SString(objectGroup.attribute("class").as_string()) == "win")
                    {
                        c1->ctype = ColliderType::WIN;
                    }
                    else
                    {
                        c1->ctype = ColliderType::UNKNOWN;
                    }

                }
                else if (SString(collider.attribute("type").as_string()) == "polygon")
                {
                    /* int* points = new int[collider.child("polygon").attribute("points").as_int() * sizeof(int)];

                        app->physics->CreateChain(collider.attribute("x").as_int(),
                                                collider.attribute("y").as_int(),
                                                points,
                                                collider.child("polygon").attribute("points").as_int(),
                                                    STATIC);*/
                }
            }
        }
        
        if (objectType == "item") {
            pugi::xml_node item;
            for (item = objectGroup.child("object"); item && ret; item = item.next_sibling("object"))
            { 

               /* if (SString(item.attribute("type").as_string()) == "coin") {
                   ScoreItem* itemEntity = (ScoreItem*)app->entityManager->CreateEntity(EntityType::SCOREITEM);
                   
                   itemEntity->position.x = item.attribute("x").as_int();
                   itemEntity->position.y = item.attribute("y").as_int();
                   itemEntity->texturePath = "Assets/Textures/bridge.png";
                   //itemEntity->parameters = item;
               } else if (SString(item.attribute("type").as_string()) == "food") {
                   FoodItem* foodItem = (FoodItem*)app->entityManager->CreateEntity(EntityType::FOODITEM);

                   foodItem->position.x = item.attribute("x").as_int();
                   foodItem->position.y = item.attribute("y").as_int();
               }
                */
            }
        }

        /* if (objectType == "checkpoint") {
            pugi::xml_node checkpoint;
            for (checkpoint = objectGroup.child("object"); checkpoint && ret; checkpoint = checkpoint.next_sibling("object"))
            {
                Checkpoint* checkpointEntity = (Checkpoint*)app->entityManager->CreateEntity(EntityType::CHECKPOINT);

                checkpointEntity->position.x = checkpoint.attribute("x").as_int();
                checkpointEntity->position.y = checkpoint.attribute("y").as_int();
                checkpointEntity->size.x = checkpoint.attribute("width").as_int();
                checkpointEntity->size.y = checkpoint.attribute("height").as_int();
                checkpointEntity->texturePath = "Assets/Textures/bridge.png";
            }
        } */
    }

    return ret;
}

bool Map::LoadLights(pugi::xml_node mapFile)
{
    bool ret = true; 
    uint scale = app->win->GetScale();

    pugi::xml_node objectGroup;
    for (objectGroup = mapFile.child("map").child("objectgroup"); objectGroup && ret; objectGroup = objectGroup.next_sibling("objectgroup"))
    {
        if(SString(objectGroup.attribute("class").as_string()) == "lighting")
        {
            pugi::xml_node properties = objectGroup.child("properties");
            pugi::xml_node property = properties.child("property");
            SString ambientColorProperty = property.attribute("value").as_string();
            
            if (ambientColorProperty == "")
            {
                app->lighting->SetAmbientLight({ 255, 255, 255, 255 });
            }else
            {
                SDL_Color ambientColor;
                sscanf(ambientColorProperty.GetString(), "#%02hhx%02hhx%02hhx%02hhx", &ambientColor.a, &ambientColor.r, &ambientColor.g, &ambientColor.b);
                app->lighting->SetAmbientLight(ambientColor);
            }

            pugi::xml_node light;
            for (light = objectGroup.child("object"); light && ret; light = light.next_sibling("object"))
            {
                float radius = light.attribute("width").as_float() / 256;
                iPoint pos = { light.attribute("x").as_int() + light.attribute("width").as_int() / 2, light.attribute("y").as_int() + light.attribute("width").as_int() / 2 };
                SString colorProperty = light.child("properties").child("property").attribute("value").as_string();
                SDL_Color color;

                if(colorProperty == "")
                {
                    color = { 255, 255, 255, 255 };
                }
                else
                {
                    sscanf(colorProperty.GetString(), "#%02hhx%02hhx%02hhx%02hhx", &color.a, &color.r, &color.g, &color.b);
                }

                app->lighting->AddLight(pos, radius, color);
            }
        }
    }

    return true;
}

 // L13: Create navigationMap map for pathfinding
void Map::CreateNavigationMap(int& width, int& height, uchar** buffer) const
{
    bool ret = false;

    //Sets the size of the map. The navigation map is a unidimensional array 
    uchar* navigationMap = new uchar[navigationLayer->width * navigationLayer->height];
    //reserves the memory for the navigation map
    memset(navigationMap, 1, navigationLayer->width * navigationLayer->height);

    for (int x = 0; x < mapData.width; x++)
    {
        for (int y = 0; y < mapData.height; y++)
        {
            //i is the index of x,y coordinate in a unidimensional array that represents the navigation map
            int i = (y * navigationLayer->width) + x;

            //Gets the gid of the map in the navigation layer
            int gid = navigationLayer->Get(x, y);
            
            //If the gid is a blockedGid is an area that I cannot navigate, so is set in the navigation map as 0, all the other areas can be navigated
            //!!!! make sure that you assign blockedGid according to your map
            if (gid == blockedGid) navigationMap[i] = 0;
            else navigationMap[i] = 1;
        }
    }

    *buffer = navigationMap;
    width = mapData.width;
    height = mapData.height;

}

bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
    bool ret = false;

    for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
    {
        Properties::Property* p = new Properties::Property();
        p->name = propertieNode.attribute("name").as_string();
        SString type = propertieNode.attribute("type").as_string();
        if(type == "bool")
        {
            p->value = propertieNode.attribute("value").as_bool();
        }
        else if(type == "int")
        {
            p->value = propertieNode.attribute("value").as_int();
        }

        properties.list.Add(p);
    }

    return ret;
}

Properties::Property* Properties::GetProperty(const char* name)
{
    ListItem<Property*>* item = list.start;
    Property* p = NULL;

    while (item)
    {
        if (item->data->name == name) {
            p = item->data;
            break;
        }
        item = item->next;
    }

    return p;
}


