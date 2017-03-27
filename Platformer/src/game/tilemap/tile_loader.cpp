#include "./tile_loader.h"

#include <SFML/Graphics.hpp>

#include <cassert>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

#include "../../log.h"

TiledJSONLoader::TiledJSONLoader(const std::string &pathToMaps, const std::string &pathToTilesets) :
  mPathToMaps(pathToMaps),
  mPathToTilesets(pathToTilesets),
  mMapWidth(0),
  mMapHeight(0),
  mTileWidth(0),
  mTileHeight(0),
  mMapLoaded(false) {
}

sf::Vector2i TiledJSONLoader::getMapSize() {
  return sf::Vector2i(mMapWidth, mMapHeight);
}

sf::Vector2i TiledJSONLoader::getTileSize() {
  return sf::Vector2i(mTileWidth, mTileHeight);
}

Tilesets TiledJSONLoader::getTilesets() {
  return mTilesets;
}

TileLayers TiledJSONLoader::getTileLayers() {
  return mTileLayers;
}

ObjectGroups TiledJSONLoader::getObjectGroups() {
  return mObjectGroups;
}

bool TiledJSONLoader::isMapLoaded() {
  return mMapLoaded;
}

void TiledJSONLoader::load(const std::string &file) {
  gamelog(DEBUG) << "Loading map file" << std::endl;
  nlohmann::json mapObject = loadFile(file);
  gamelog(DEBUG) << "Parsing map attributes" << std::endl;
  loadMapAttributes(mapObject);
  gamelog(DEBUG) << "Parsing tilesets" << std::endl;
  loadTilesets(mapObject);
  gamelog(DEBUG) << "Parsing layers" << std::endl;
  loadLayers(mapObject);
  gamelog(DEBUG) << "Map loaded" << std::endl;
  mMapLoaded = true;
}

nlohmann::json TiledJSONLoader::loadFile(const std::string &file) {
  std::string mapFile = mPathToMaps + file + ".json";
  std::ifstream is(mapFile.c_str());

  std::stringstream buffer;
  buffer << is.rdbuf();
  std::string data = buffer.str();

  return nlohmann::json::parse(data);
}

void TiledJSONLoader::loadMapAttributes(nlohmann::json &map) {
  mMapWidth = map["width"];
  mMapHeight = map["height"];
  mTileWidth = map["tilewidth"];
  mTileHeight = map["tileheight"];
}

void TiledJSONLoader::loadTilesets(nlohmann::json &map) {
  nlohmann::json tilesetsVal = map["tilesets"];
  for (auto &tilesetVal : tilesetsVal) {
    nlohmann::json tilesetMap = tilesetVal;
    Tileset currentTileset;
    currentTileset.name = tilesetMap["name"].get<std::string>();
    currentTileset.file = tilesetMap["image"].get<std::string>();
    currentTileset.firstGID = tilesetMap["firstgid"];
    currentTileset.imageHeight = tilesetMap["imageheight"];
    currentTileset.imageWidth = tilesetMap["imagewidth"];
    currentTileset.margin = tilesetMap["margin"];
    currentTileset.spacing = tilesetMap["spacing"];
    mTilesets.push_back(currentTileset);

    //Load our tileset
    sf::Texture texture;
    if (!texture.loadFromFile(mPathToTilesets + currentTileset.file))
      throw std::runtime_error("Tiled JSON Loader failed to read tileset " + currentTileset.file);
    mTilesetTextures[currentTileset.name] = texture;
  }
}

void TiledJSONLoader::loadLayers(nlohmann::json &map) {
  nlohmann::json layers = map["layers"];
  for (auto &layerVal : layers) {
    nlohmann::json layer = layerVal;
    std::string type = layer["type"];
    if (type.compare("tilelayer") == 0)
      loadTileLayer(layer, mTilesets);
    if (type.compare("objectgroup") == 0)
      loadObjectGroup(layer, mTilesets);
  }
}

void TiledJSONLoader::loadTileLayer(nlohmann::json &layer, Tilesets &tilesets) {
  TileLayer currentLayer;
  currentLayer.name = layer["name"].get<std::string>();
  currentLayer.visible = layer["visible"];
  currentLayer.opacity = layer["opacity"];
  currentLayer.tiles = loadTiles(layer, tilesets);
  mTileLayers.push_back(currentLayer);
}

TileGrid TiledJSONLoader::loadTiles(nlohmann::json &layer, Tilesets &tilesets) {
  nlohmann::json tileArray = layer["data"];

  //Initial fill of empty tiles
  TileGrid layerGrid;
  for (int row = 0; row < mMapHeight; ++row) {
    Tiles currentRow;
    for (int col = 0; col < mMapWidth; ++col) {
      currentRow.push_back(Tile());
    }
    layerGrid.push_back(currentRow);
  }

  int mapTileNum = 0;
  int mapRow = 0;
  for (auto &tile : tileArray) {
    Tile currentTile = loadTile(tile, tilesets);
    Tileset tileset = getTilesetFromGID(currentTile.GID, tilesets);
    currentTile.sprite.setPosition(
      static_cast<float>((mapTileNum % mMapWidth) * (mTileWidth)+tileset.margin),
      static_cast<float>((mapRow)* (mTileHeight)+tileset.margin));
    layerGrid[mapRow][mapTileNum % mMapWidth] = currentTile;
    if ((mapTileNum + 1) % mMapWidth == 0) {
      ++mapRow;
    }
    ++mapTileNum;
  }
  return layerGrid;
}

Tile TiledJSONLoader::loadTile(nlohmann::json &tileVal, Tilesets &tilesets) {
  Tile currentTile;
  unsigned int GID = tileVal;
  currentTile.GID = GID;
  if (GID == 0) return currentTile;

  Tileset tileset = getTilesetFromGID(GID, tilesets);
  int tilesPerRow = static_cast<int>
    (std::floor(static_cast<float>(tileset.imageWidth - tileset.margin) / (mTileWidth + tileset.spacing)));
  int tilesetRow = static_cast<int>(std::ceil(static_cast<float>(GID - tileset.firstGID + 1) / tilesPerRow));
  int tilesetCol = tilesPerRow * (1 - tilesetRow) + (GID - tileset.firstGID + 1);
  sf::Vector2i spriteSheetPos(
    ((tilesetCol - 1) * (mTileWidth + tileset.spacing) + tileset.margin - 1),
    ((tilesetRow - 1) * (mTileHeight + tileset.spacing) + tileset.margin - 1));
  sf::Texture &tilesetTexture = mTilesetTextures[tileset.name];
  sf::Sprite tileSprite;
  tileSprite.setTexture(tilesetTexture);
  tileSprite.setTextureRect(sf::IntRect(spriteSheetPos, sf::Vector2i(mTileWidth + 1, mTileHeight + 1)));
  currentTile.sprite = tileSprite;
  return currentTile;
}

void TiledJSONLoader::loadObjectGroup(nlohmann::json &group, Tilesets &tilesets) {
  ObjectGroup currentGroup;
  currentGroup.name = group["name"].get<std::string>();
  currentGroup.visible = group["visible"];
  currentGroup.opacity = group["opacity"];
  currentGroup.objects = loadObjects(group, tilesets);
  mObjectGroups.push_back(currentGroup);
}

Objects TiledJSONLoader::loadObjects(nlohmann::json &group, Tilesets &tilesets) {
  nlohmann::json objectArray = group["objects"];

  Objects objects;
  for (auto &object : objectArray) {
    objects.push_back(loadObject(object, tilesets));
  }
  return objects;
}

Object TiledJSONLoader::loadObject(nlohmann::json &objectVal, Tilesets &tilesets) {
  Object currentObject;
  nlohmann::json object = objectVal;
  unsigned int GID;
  if (object.find("gid") == object.end())
    GID = 0;
  else
    GID = object["gid"];
  currentObject.GID = GID;
  currentObject.name = object["name"].get<std::string>();
  currentObject.type = object["type"].get<std::string>();
  currentObject.properties = object["properties"].get<std::map<std::string, nlohmann::json>>();
  currentObject.position = sf::Vector2f(
    static_cast<float>(object["x"]),
    static_cast<float>(object["y"]));
  if (GID == 0) return currentObject;

  Tileset tileset = getTilesetFromGID(GID, tilesets);
  int tilesPerRow = static_cast<int>
    (std::floor(static_cast<float>
      (tileset.imageWidth - tileset.margin) / (mTileWidth + tileset.spacing)));
  int tilesetRow = static_cast<int>
    (std::ceil(static_cast<float>(GID - tileset.firstGID) / tilesPerRow));
  int tilesetCol = tilesPerRow * (1 - tilesetRow) + (GID - tileset.firstGID + 1);

  if (GID == tileset.firstGID) {
    tilesetRow = 1;
    tilesetCol = 1;
  }

  sf::Vector2i spriteSheetPos(
    ((tilesetCol - 1) * (mTileWidth + tileset.spacing) + tileset.margin - 1), (
      (tilesetRow - 1) * (mTileHeight + tileset.spacing) + tileset.margin - 1));
  sf::Texture &tilesetTexture = mTilesetTextures[tileset.name];
  sf::Sprite objectSprite;
  objectSprite.setTexture(tilesetTexture);
  objectSprite.setTextureRect(sf::IntRect(spriteSheetPos, sf::Vector2i(mTileWidth + 1, mTileHeight + 1)));
  objectSprite.setPosition(sf::Vector2f(currentObject.position.x, currentObject.position.y - mTileHeight));

  currentObject.sprite = objectSprite;

  return currentObject;
}

Tileset TiledJSONLoader::getTilesetFromGID(unsigned int GID, Tilesets &tilesets) {
  Tileset tileset;
  for (unsigned int i = 0; i < tilesets.size(); ++i) {
    if ((i == tilesets.size() - 1) || ((GID >= tilesets[i].firstGID) &&
      (GID < tilesets[i + 1].firstGID))) {
      return tilesets[i];
    }
  }
  return tilesets[0];
}