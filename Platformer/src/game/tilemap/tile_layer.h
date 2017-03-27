#ifndef TILE_LAYER_H__
#define TILE_LAYER_H__

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "./Tile.h"

class TileLayer;

using TileLayers = std::vector<TileLayer>;

class TileLayer {
public:
  TileGrid tiles;
  std::string name;
  bool visible;
  double opacity;
};

#endif TILE_LAYER_H__