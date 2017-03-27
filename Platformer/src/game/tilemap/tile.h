#ifndef TILE_H__
#define TILE_H__

#include <SFML/Graphics.hpp>

#include <vector>
#include <string>

class Tile;

using Tiles = std::vector<Tile>;
using TileGrid = std::vector<Tiles>;

class Tile {
public:
  Tile() : GID(0) {}

public:
  unsigned int GID;
  sf::Sprite sprite;
};

#endif  // TILE_H__
