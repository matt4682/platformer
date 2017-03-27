#ifndef TILE_LOADER_H__
#define TILE_LOADER_H__

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "../../../lib/json/src/json.hpp"

#include "./tileset.h"
#include "./tile_layer.h"
#include "./object_group.h"

class TiledJSONLoader {
public:
  TiledJSONLoader(const std::string &pathToMaps, const std::string &pathToTilesets);

public:
  void load(const std::string &file);
  sf::Vector2i getMapSize();
  sf::Vector2i getTileSize();
  Tilesets getTilesets();
  TileLayers getTileLayers();
  ObjectGroups getObjectGroups();
  bool isMapLoaded();

private:
  std::string mPathToMaps;
  std::string mPathToTilesets;
  bool mMapLoaded;
  int mMapWidth;
  int mMapHeight;
  int mTileWidth;
  int mTileHeight;
  Tilesets mTilesets;
  TileLayers mTileLayers;
  ObjectGroups mObjectGroups;
  std::map<std::string, sf::Texture> mTilesetTextures;

private:
  nlohmann::json loadFile(const std::string &file);
  void loadMapAttributes(nlohmann::json &map);
  void loadTilesets(nlohmann::json &map);
  void loadLayers(nlohmann::json &map);
  void loadTileLayer(nlohmann::json &layer, Tilesets &tilesets);
  TileGrid loadTiles(nlohmann::json &layer, Tilesets &tilesets);
  Tile loadTile(nlohmann::json &tileVal, Tilesets &tilesets);
  void loadObjectGroup(nlohmann::json &group, Tilesets &tilesets);
  Objects loadObjects(nlohmann::json &group, Tilesets &tilesets);
  Object loadObject(nlohmann::json &objectVal, Tilesets &tilesets);
  Tileset getTilesetFromGID(unsigned int GID, Tilesets &tilesets);
};

#endif  // TILE_LOADER_H__