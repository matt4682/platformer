#ifndef SAVE_MANAGER_H__
#define SAVE_MANAGER_H__

#include <memory>
#include <map>
#include <iostream>
#include <fstream>
#include <vector>

#include "../lib/json/src/json.hpp"

using LevelData = std::tuple<std::string, float, bool, std::string>;

enum LevelDataIndex {
  LevelName,
  TargetTime,
  HasGem,
  WorldName
};

using UserData = std::tuple<float, bool, bool, bool>;

enum UserDataIndex {
  BestTime,
  BeatLevel,
  BeatTargetTime,
  AcquiredGem
};

using LastCompletedData =
std::tuple<std::string, float, bool, bool, unsigned, std::string>;

enum LastDataIndex {
  MapName,
  BeatTime,
  BeatTarget,
  GotGem,
  Deaths,
  WorldKey
};

using Level = std::pair<LevelData, UserData>;

class SaveManager {
public:
  static SaveManager *sharedInstance();

  void load(nlohmann::json data);
  // void loadSaveFile();

  nlohmann::json save();
  void writeSaveFile();

  void finishLevel(const std::string &level,
    const unsigned deaths,
    const float time,
    const bool gem);
  bool levelCompleted(const std::string &level);
  LastCompletedData lastCompletedData();
  std::map<std::string, int>  worldMap();
  std::map<std::string, Level> levelMap();

private:
  static SaveManager *m_instance;

  SaveManager();
  ~SaveManager() = default;
  SaveManager(const SaveManager &other) = delete;
  SaveManager(SaveManager &&other) = delete;
  SaveManager &operator=(const SaveManager &other) = delete;
  SaveManager &operator=(SaveManager &&other) = delete;

  std::map<std::string, Level> m_level_map;
  std::map<std::string, int> m_world_map;
  LastCompletedData m_last_completed;
};

#endif  // SAVE_MANAGER_H__