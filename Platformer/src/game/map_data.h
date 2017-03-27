#ifndef MAP_DATA_H__
#define MAP_DATA_H__

#include "./tilemap/tile_layer.h"
#include "./tilemap/object_group.h"

class MapData {
public:
  int mapWidth;
  int mapHeight;
  int tileWidth;
  int tileHeight;
  TileLayers tileLayers;
  ObjectGroups objectGroups;
};

#endif  // MAP_DATA_H__