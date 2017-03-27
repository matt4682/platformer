#ifndef LEVEL_MANAGER_H__
#define LEVEL_MANAGER_H__

#include <SFML\Graphics.hpp>
#include <map>
#include <string>

#include "./sound_player.h"
#include "../ui/state.h"

class World;

class LevelManager {
public:
  LevelManager();

  void insert(State::Context context, std::string &map);
  void load(const std::string &map);
  World *get(const std::string &map);

private:
  std::map<std::string, World *> mLevelMap;
};

#endif  // LEVEL_MANAGER_H__
