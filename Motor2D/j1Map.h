#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"

struct Properties
{
	struct Property
	{
		p2SString name;
		int value;
	};

	~Properties()
	{
		p2List_item<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.clear();
	}

	int Get(const char* name, int default_value = 0) const;

	p2List<Property*>	list;
};

// ----------------------------------------------------
struct MapLayer
{
	p2SString name = nullptr;
	uint width = 0, height = 0;
	bool logic = false;
	uint* gid = nullptr;
	Properties properties;

	~MapLayer()
	{
		RELEASE(gid);
	}

	inline uint Get(int x, int y) const
	{
		return gid[(y*width) + x];
	}

};

// ----------------------------------------------------
struct TileSet
{

	SDL_Rect GetTileRect(int id) const;

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture  = nullptr;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

enum LogicData
{
	NONE,
	WALL,
	DANGER,
	START
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<MapLayer*>   layers;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	//Delete Map
	bool DeleteMap();

	//Map Logic
	void SetMapLogic();

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	TileSet* GetTilesetFromTileId(int id) const;

public:

	MapData data;
	MapLayer LayerInfo;
	Uint32				num_elements = 0;
	p2List_item<MapLayer*>* logic_layer;

	int					numelements;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded = false;

	MapLayer*			logicLayer = nullptr;
	MapLayer*			parallax1Layer = nullptr;
	MapLayer*			parallax2Layer = nullptr;

	enum Logic
	{
		DEAD = 4,
		SPAWN_P = 14,
		WALL = 19,
		SPAWN_FE = 26,
		SPAWN_WE = 27,
		LEVEL_2 = 9,
		HIDDEN_LEVEL = 24,
		URNS = 16,
		
	};

};

#endif // __j1MAP_H__