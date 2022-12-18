
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Physics.h"
#include "Scene.h"
#include "EntityManager.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>
#include "SDL_image/include/SDL_image.h"

Map::Map() : Module(), mapLoaded(false)
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

    //mapFileName = config.child("mapfile").attribute("path").as_string();
    mapFolder = config.child("mapfolder").attribute("path").as_string();

    return ret;
}

void Map::Draw()
{
    if(mapLoaded == false)
        return;

    ListItem<MapLayer*>* mapLayerItem;
    mapLayerItem = mapData.maplayers.start;

    while (mapLayerItem != NULL) {

        //L06: DONE 7: use GetProperty method to ask each layer if your “Draw” property is true.
        if (mapLayerItem->data->properties.GetProperty("Draw") != NULL && mapLayerItem->data->properties.GetProperty("Draw")->value) {

            for (int x = 0; x < mapLayerItem->data->width; x++)
            {
                for (int y = 0; y < mapLayerItem->data->height; y++)
                {
                    // L05: DONE 9: Complete the draw function
                    int gid = mapLayerItem->data->Get(x, y);

                    //L06: DONE 3: Obtain the tile set using GetTilesetFromTileId
                    TileSet* tileset = GetTilesetFromTileId(gid);

                    SDL_Rect r = tileset->GetTileRect(gid);
                    iPoint pos = MapToWorld(x, y);

                    app->render->DrawTexture(tileset->texture,
                        pos.x,
                        pos.y,
                        &r);
                }
            }
        }
        mapLayerItem = mapLayerItem->next;

    }
}

// L05: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
    iPoint ret;

    ret.x = x * mapData.tileWidth;
    ret.y = y * mapData.tileHeight;

    return ret;
}

iPoint Map::WorldToMap(int x, int y)
{
    iPoint ret(0, 0);

    if (mapData.type == MAPTYPE_ORTHOGONAL)
    {
        ret.x = x / mapData.tileWidth;
        ret.y = y / mapData.tileHeight;
    }
    else if (mapData.type == MAPTYPE_ISOMETRIC)
    {
        float halfWidth = mapData.tileWidth * 0.5f;
        float halfHeight = mapData.tileHeight * 0.5f;
        ret.x = int((x / halfWidth + y / halfHeight) / 2);
        ret.y = int((y / halfHeight - x / halfWidth) / 2);
    }
    else
    {
        LOG("Unknown map type");
        ret.x = x; ret.y = y;
    }

    return ret;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int gid) const
{
    SDL_Rect rect = { 0 };
    int relativeIndex = gid - firstgid;

    // L05: DONE 7: Get relative Tile rectangle
    rect.w = tileWidth;
    rect.h = tileHeight;
    rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
    rect.y = margin + (tileWidth + spacing) * (relativeIndex / columns);

    return rect;
}

Tile* TileSet::GetTile(int gid) const
{
    int relativeIndex = gid - firstgid;
    ListItem<Tile*>* item = extraTileInformation.start;
    Tile* tile = NULL;
    while (item)
    {
        if (item->data->gid == relativeIndex && item->data->isAnimated)
        {
            tile = item->data;
        }
        item = item->next;
    }
    return tile;
}

// L06: DONE 2: Pick the right Tileset based on a tile id
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

    // L04: DONE 2: Make sure you clean up any memory allocated from tilesets/map
	ListItem<TileSet*>* item;
	item = mapData.tilesets.start;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	mapData.tilesets.Clear();

    // L05: DONE 2: clean up all layer data
    // Remove all layers
    ListItem<MapLayer*>* layerItem;
    layerItem = mapData.maplayers.start;

    while (layerItem != NULL)
    {
        RELEASE(layerItem->data);
        layerItem = layerItem->next;
    }
    mapData.maplayers.Clear();

    ListItem<ObjectGroup*>* groupItem;
    groupItem = mapData.objectGroups.start;

    while (groupItem != NULL)
    {
        
        RELEASE(groupItem->data);
        /*if (groupItem->data->objects != NULL)
            delete[] groupItem->data->objects;

        delete groupItem->data;*/

        groupItem = groupItem->next;
    }
    mapData.objectGroups.Clear();

    return true;
}

// Load new map
bool Map::Load(const char* fileName)
{
    bool ret = true;

    mapFileName = fileName;


    SString path("%s%s.tmx", mapFolder.GetString(), mapFileName.GetString());

    pugi::xml_document mapFileXML;
    pugi::xml_parse_result result = mapFileXML.load_file(path.GetString());

    if(result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", mapFileName, result.description());
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

    // L05: DONE 4: Iterate all layers and load each of them
    if (ret == true)
    {
        ret = LoadAllLayers(mapFileXML.child("map"));
    }
    
    // L07 DONE 3: Create colliders
    // Later you can create a function here to load and create the colliders from the map


    if (ret == true)
    {
        ret = LoadAllObjectGroups(mapFileXML.child("map"));
        //pugi::xml_node test = app->scene->player->parameters;
    }

    if(ret == true)
    {
        // L04: DONE 5: LOG all the data loaded iterate all tilesets and LOG everything
       
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

        // L05: DONE 4: LOG the info for each loaded layer
        ListItem<MapLayer*>* mapLayer;
        mapLayer = mapData.maplayers.start;

        while (mapLayer != NULL) {
            LOG("id : %d name : %s", mapLayer->data->id, mapLayer->data->name.GetString());
            LOG("Layer width : %d Layer height : %d", mapLayer->data->width, mapLayer->data->height);
            mapLayer = mapLayer->next;
        }
    }

    if(mapFileXML) mapFileXML.reset();

    mapLoaded = ret;

    return ret;
}

// L04: DONE 3: Implement LoadMap to load the map properties
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
        // L08: DONE 2: Read the prientation of the map
        mapData.type = MAPTYPE_UNKNOWN;
        if (strcmp(map.attribute("orientation").as_string(), "isometric") == 0)
        {
            mapData.type = MAPTYPE_ISOMETRIC;
        }
        if (strcmp(map.attribute("orientation").as_string(), "orthogonal") == 0)
        {
            mapData.type = MAPTYPE_ORTHOGONAL;
        }
    }
    return ret;
}

// L04: DONE 4: Implement the LoadTileSet function to load the tileset properties
bool Map::LoadTileSet(pugi::xml_node mapFile){

    bool ret = true; 

    pugi::xml_node tileset;
    for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
    {
        TileSet* set = new TileSet();

        // L04: DONE 4: Load Tileset attributes
        set->name = tileset.attribute("name").as_string();
        set->firstgid = tileset.attribute("firstgid").as_int();
        set->margin = tileset.attribute("margin").as_int();
        set->spacing = tileset.attribute("spacing").as_int();
        set->tileWidth = tileset.attribute("tilewidth").as_int();
        set->tileHeight = tileset.attribute("tileheight").as_int();
        set->columns = tileset.attribute("columns").as_int();
        set->tilecount = tileset.attribute("tilecount").as_int();

        // L04: DONE 4: Load Tileset image
        SString tmp("%s%s", mapFolder.GetString(), tileset.child("image").attribute("source").as_string());
        set->texture = app->tex->Load(tmp.GetString());

        ret = LoadTileExtraInformation(tileset, set);

        mapData.tilesets.Add(set);
    }

    return ret;
}

bool Map::LoadTileExtraInformation(pugi::xml_node& node, TileSet* set) 
{
    bool ret = true;

    pugi::xml_node tileNode;
    for (tileNode = node.child("tile"); tileNode && ret; tileNode = tileNode.next_sibling("tile"))
    {
        Tile* tile = new Tile();

        set->extraTileInformation.Add(tile);
        tile->gid = tileNode.attribute("id").as_int();

        // Load collider information
        pugi::xml_node groupNode = tileNode.child("objectgroup").child("object");
        //problems with collider
        if (false)
        {
            tile->colliderPos = { groupNode.attribute("x").as_float(), groupNode.attribute("y").as_float() };
            tile->height = groupNode.attribute("height").as_float();
            tile->width = groupNode.attribute("width").as_float();
        }
        else
        {
            tile->colliderPos = { 0,0 };
            tile->height = 7;
            tile->width = 7;
        }

        ret = LoadAnimation(tileNode.child("animation"), tile, set);

    }

    return ret;
}

bool Map::LoadAnimation(pugi::xml_node& node, Tile* tile, TileSet* set)
{
    bool ret = true;

    pugi::xml_node animNode;
    for (animNode = node.child("frame"); animNode && ret; animNode = animNode.next_sibling("frame"))
    {
        tile->isAnimated = true;
        tile->animation.PushBack(set->GetTileRect(animNode.attribute("tileid").as_int() + set->firstgid));
    }


    return true;
}

// L05: DONE 3: Implement a function that loads a single layer layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
    bool ret = true;

    //Load the attributes
    layer->id = node.attribute("id").as_int();
    layer->name = node.attribute("name").as_string();
    layer->width = node.attribute("width").as_int();
    layer->height = node.attribute("height").as_int();

    //L06: DONE 6 Call Load Propoerties
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

// L05: DONE 4: Iterate all layers and load each of them

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

bool Map::LoadCollisions(pugi::xml_node& node, ObjectGroup* group)
{
    bool ret = true;

    int i = 0;
    for (pugi::xml_node objectNode = node.child("object"); objectNode; objectNode = objectNode.next_sibling("object"), i++)
    {
        group->objects[i].name = node.attribute("name").as_string();
        group->objects[i].id = node.attribute("id").as_int();

        float x = round(objectNode.attribute("x").as_float());
        float y = round(objectNode.attribute("y").as_float());
        float width = round(objectNode.attribute("width").as_float());
        float height = round(objectNode.attribute("height").as_float());
        SString shape = objectNode.first_child().name();
        SString type = objectNode.attribute("class").as_string();
        
        PhysBody* pBody = app->physics->CreateRectangle(x + round(width / 2), y +round(height / 2), width, height, STATIC);

        if (type == "Platform")
        {
            pBody->ctype = ColliderType::PLATFORM;
        }
        else if (type == "Water") 
        {
            pBody->ctype = ColliderType::WATER;
        }
        else if (type == "Wall")
        {
            pBody->ctype = ColliderType::WALL;
        }
        else
        {
            pBody->ctype = ColliderType::UNKNOWN;
        }
        group->objects[0].pBody = pBody;
    }

    return ret;
}

bool Map::LoadObjectGroup(pugi::xml_node& node, ObjectGroup* group)
{
    bool ret = true;
    group->name = node.attribute("name").as_string();

    int amountObjects = 0;
    for (pugi::xml_node iteratorNode = node.child("object"); iteratorNode; iteratorNode = iteratorNode.next_sibling("object"), amountObjects++) {}

    group->objectsSize = amountObjects;
    group->objects = new Object[amountObjects];
    memset(group->objects, 0, amountObjects * sizeof(Object));


    if (group->name == "Collisions")
    {
        LoadCollisions(node, group);
    }
    if (group->name == "Entities") {
        LoadEntities(node);
    }
    
    return ret;
    
}

bool Map::LoadEntities(pugi::xml_node& node)
{
    bool ret = true;

    for (pugi::xml_node objectNode = node.child("object"); objectNode; objectNode = objectNode.next_sibling("object"))
    {

        SString name = objectNode.attribute("name").as_string();

        //Load Player
        if (name == "Player") {
            app->scene->player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER, objectNode);

        }
        else {
            app->entityManager->CreateEntity(EntityType::ENEMY, objectNode);
        }
    }

    //pugi::xml_node test = app->scene->player->parameters;

    return ret;
}

bool Map::LoadAllObjectGroups(pugi::xml_node mapNode)
{

    bool ret = true;

    for (pugi::xml_node groupNode = mapNode.child("objectgroup"); groupNode && ret; groupNode = groupNode.next_sibling("objectgroup"))
    {
        //Load the layer
        ObjectGroup* objectGroup = new ObjectGroup();
        ret = LoadObjectGroup(groupNode, objectGroup);
        
        mapData.objectGroups.Add(objectGroup);

        //add the objectgroup to the map
        //mapData.maplayers.Add(mapLayer);
    }

    pugi::xml_node test = app->scene->player->parameters;
    return ret;

}

bool Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
    bool ret = false;
    ListItem<MapLayer*>* item;
    item = mapData.maplayers.start;

    for (item = mapData.maplayers.start; item != NULL; item = item->next)
    {
        MapLayer* layer = item->data;

        if (!layer->properties.GetProperty("Navigation")->value)
            continue;

        uchar* map = new uchar[layer->width * layer->height];
        memset(map, 1, layer->width * layer->height);

        for (int y = 0; y < mapData.height; ++y)
        {
            for (int x = 0; x < mapData.width; ++x)
            {
                int i = (y * layer->width) + x;

                int tileId = layer->Get(x, y);
                TileSet* tileset = (tileId > 0) ? GetTilesetFromTileId(tileId) : NULL;

                if (tileset != NULL)
                {
                    map[i] = (tileId - tileset->firstgid) > 0 ? 0 : 1;
                }
            }
        }

        *buffer = map;
        width = mapData.width;
        height = mapData.height;
        ret = true;

        break;
    }

    return ret;
}

// L06: DONE 6: Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
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


// L06: DONE 7: Ask for the value of a custom property
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


