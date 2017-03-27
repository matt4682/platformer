#ifndef TILESET_H__
#define TILESET_H__

#include <vector>
#include <string>

class Tileset;

using Tilesets = std::vector<Tileset>;

class Tileset {
public:
  unsigned int firstGID;
  std::string file;
  std::string name;
  int imageHeight;
  int imageWidth;
  int margin;
  int spacing;
};

#endif  // TILESET_H__