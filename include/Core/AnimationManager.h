//#ifndef __MAP_H__
//#define __MAP_H__
//
//#include "Core/Module.h"
//#include "Utils/List.h"
//#include "Utils/Point.h"
//
//#include <pugixml.hpp>
//#include <SDL.h>
//
//#include "Utils/Defs.h"
//#include "Utils/Log.h"
//
//class Animation;
//
//// Ignore Terrain Types and Tile Types for now, but we want the image!
//struct TileSet
//{
//	SString	name;
//	int	firstgid;
//	int margin;
//	int	spacing;
//	int	tileWidth;
//	int	tileHeight;
//	int columns;
//	int tilecount;
//
//	SDL_Texture* texture;
//	SDL_Rect GetTileRect(int gid) const;
//};
//
//struct Colliders
//{
//	int x;
//	int y;
//	int width;
//	int height;
//};
//
//struct Properties
//{
//	struct Property
//	{
//		SString name;
//		bool value;
//	};
//
//	~Properties()
//	{
//		//...
//		ListItem<Property*>* item;
//		item = list.start;
//
//		while (item != NULL)
//		{
//			RELEASE(item->data);
//			item = item->next;
//		}
//
//		list.Clear();
//	}
//
//	Property* GetProperty(const char* name);
//
//	List<Property*> list;
//};
//
////struct AnimLayer
////{
////	SString	name;
////	int id;
////	int width;
////	int height;
////	float parallaxFactor;
////	uint* data;
////
////	Properties properties;
////
////	MapLayer() : data(NULL)
////	{}
////
////	~MapLayer()
////	{
////		RELEASE(data);
////	}
////
////	inline uint Get(int x, int y) const
////	{
////		return data[(y * width) + x];
////	}
////};
//
//struct AnimData
//{
//	int width;
//	int	height;
//	int	tileWidth;
//	int	tileHeight;
//	List<TileSet*> tilesets;
//	AnimTypes type;
//
//	List<Animation*> animations;
//};
//
//class AnimationManager : public Module {
//
//public:
//
//	AnimationManager();
//
//	AnimationManager(bool startEnabled);
//
//	// Destructor
//	virtual ~AnimationManager();
//
//	// Called before render is available
//	bool Awake(pugi::xml_node& conf);
//
//	// Called before the first frame
//	bool Start();
//
//	// Called each loop iteration
//	bool Update(float dt);
//
//	// Called before quitting
//	bool CleanUp();
//
//	Animation* GetAnimByName(SString name);
//
//private:
//
//	// clipping margin to prevent seeing the tiles generating in the border of the screen
//	const int clippingMargin = 2;
//
//	bool LoadAnimation(pugi::xml_node node, TileSet* tileset);
//
//public:
//	AnimData animData;
//	SString name;
//	SString animPath;
//};
//
//#endif // __ANIMATIONMANAGER_H__