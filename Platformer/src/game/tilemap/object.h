#ifndef OBJECT_H__
#define OBJECT_H__

#include <SFML/Graphics.hpp>

#include <vector>
#include <map>
#include <string>

#include "../../../lib/json/src/json.hpp"

class Object;

using Objects = std::vector<Object>;

class Object {
public:
  Object() : GID(0) {}

public:
  unsigned int GID;
  bool visible;
  std::string name;
  std::string type;
  std::map<std::string, nlohmann::json> properties;
  sf::Vector2f position;
  sf::Sprite sprite;
};

#endif  // OBJECT_H__
