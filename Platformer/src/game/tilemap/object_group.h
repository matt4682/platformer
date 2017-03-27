#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "Object.h"

class ObjectGroup;

using ObjectGroups = std::vector<ObjectGroup>;

class ObjectGroup {
public:
  Objects objects;
  std::string name;
  bool visible;
  double opacity;
};