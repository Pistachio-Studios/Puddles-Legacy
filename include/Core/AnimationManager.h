#ifndef __ANIMATIONMANAGER_H__
#define __ANIMATIONMANAGER_H__

#include "Core/Module.h"
#include "Core/Pathfinding.h"
#include "Utils/List.h"
#include "Core/Map.h"
#include "Utils/Point.h"
#include "Core/Animation.h"

#include <pugixml.hpp>
#include <SDL.h>

#include "Utils/Defs.h"
#include "Utils/Log.h"


class AnimationManager;

struct AnimData
{
	int width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	List<TileSet*> tilesets;

	List<Animation*> animations;
};

class AnimationManager : public Module {

public:

	AnimationManager();

	AnimationManager(bool startEnabled);

	// Destructor
	virtual ~AnimationManager();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	TileSet* GetTilesetFromTileId(int gid) const;

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(SString animFileName);

	bool LoadTileSet(pugi::xml_node animFile);

	Animation* GetAnimByName(SString name);

private:

	bool LoadAnimation(pugi::xml_node node, TileSet* tileset);
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

public:
	SString path;
	SString name2;

private:
	AnimData animData;
};

#endif // __ANIMATIONMANAGER_H__