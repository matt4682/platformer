#include "./save_manager.h"

#include <algorithm>
#include <exception>
#include <string>

using namespace std;
using json = nlohmann::json;

SaveManager *SaveManager::m_instance = nullptr;

SaveManager *SaveManager::sharedInstance() {
  if (!m_instance) {
    m_instance = new SaveManager;
  }
  return m_instance;
}

SaveManager::SaveManager() {
  constexpr char *save_file = "res/Maps/worlds.dat";

  ifstream ifs(save_file);
  if (ifs.good()) {
    stringstream buffer;
    buffer << ifs.rdbuf();
    auto world_data = json::parse(buffer.str());

    for (const auto &world : world_data) {
      m_world_map[world["name"]] = world["gems"];
    }
  }
  else {
    throw exception("Unable to load world.dat");
  }

  for (const auto& world : m_world_map) {
    std::string level = "res/Maps/" + world.first + "/levels.dat";
    ifs = ifstream(level);

    if (ifs.good()) {
      stringstream buffer;
      buffer << ifs.rdbuf();
      auto level_data = json::parse(buffer.str());

      for (const auto &level : level_data) {
        LevelData data;
        get<LevelDataIndex::LevelName>(data) = level.value("name", "");
        get<LevelDataIndex::HasGem>(data) = level["gem"];
        get<LevelDataIndex::TargetTime>(data) = level["time"];
        get<LevelDataIndex::WorldName>(data) = world.first;

        m_level_map[level["name"]] = make_pair(data, UserData());
      }
    }
    else {
      throw exception("Unable to load levels.dat");
    }
  }
}

////////////////////////////////////////////////////////////////////////////////

void SaveManager::load(json data) {
  for (const auto &level : data) {
    auto data_pair = m_level_map[level["name"]];
    auto level_data = data_pair.first;
    auto user_data = data_pair.second;

    float best_time = level["best_time"];
    bool got_gem = level["got_gem"];
    float target_time = get<LevelDataIndex::TargetTime>(level_data);

    get<UserDataIndex::BestTime>(user_data) = best_time;
    get<UserDataIndex::BeatLevel>(user_data) = best_time != 0.f;
    get<UserDataIndex::BeatTargetTime>(user_data) = best_time <= target_time;
    get<UserDataIndex::AcquiredGem>(user_data) = got_gem;

    m_level_map[level["name"]] = std::make_pair(level_data, user_data);
  }
}

json SaveManager::save() {
  json array;

  for (const auto &data_pair : m_level_map) {
    auto key = data_pair.first;
    auto level = data_pair.second;
    auto level_data = level.first;
    auto user_data = level.second;

    nlohmann::json element;

    element["best_time"] = get<UserDataIndex::BestTime>(user_data);
    element["got_gem"] = get<UserDataIndex::AcquiredGem>(user_data);
    element["name"] = get<LevelDataIndex::LevelName>(level_data);

    array[key] = element;
  }

  return array;
}

void SaveManager::writeSaveFile() {
  constexpr char *save_file = "save.dat";

  std::ofstream ofs(save_file);

  if (ofs.good()) {
    stringstream buffer;
    buffer << save();

    ofs << buffer.str();
  }
}

////////////////////////////////////////////////////////////////////////////////

void SaveManager::finishLevel(const std::string &t_level,
  const unsigned t_deaths,
  const float t_time,
  const bool t_gem) {
  if (t_level.empty()) return;
  auto key = t_level.substr(t_level.find("/") + 1);
  auto level = m_level_map[key];
  auto level_data = level.first;
  auto user_data = level.second;

  bool has_gem = get<LevelDataIndex::HasGem>(level_data);
  bool got_gem = get<UserDataIndex::AcquiredGem>(user_data);
  float best_time = get<UserDataIndex::BestTime>(user_data);
  float target_time = get<LevelDataIndex::TargetTime>(level_data);
  std::string world = t_level.substr(0, t_level.find("/"));

  if (best_time == 0.f) {
    best_time = t_time;
  }

  get<UserDataIndex::BestTime>(user_data) = min(best_time, t_time);
  get<UserDataIndex::BeatTargetTime>(user_data) = best_time <= target_time;
  get<UserDataIndex::BeatLevel>(user_data) = true;
  get<UserDataIndex::AcquiredGem>(user_data) = got_gem || t_gem || !has_gem;

  get<LastDataIndex::BeatTime>(m_last_completed) = t_time;
  get<LastDataIndex::BeatTarget>(m_last_completed) = t_time <= target_time;
  get<LastDataIndex::GotGem>(m_last_completed) = got_gem || t_gem || !has_gem;
  get<LastDataIndex::Deaths>(m_last_completed) = t_deaths;
  get<LastDataIndex::MapName>(m_last_completed) = key;
  get<LastDataIndex::WorldKey>(m_last_completed) = world;

  m_level_map[key] = make_pair(level_data, user_data);

  writeSaveFile();
}

LastCompletedData SaveManager::lastCompletedData() {
  return m_last_completed;
}

bool SaveManager::levelCompleted(const std::string &t_level) {
  return get<UserDataIndex::BeatLevel>(m_level_map[t_level].second);
}

map<string, int> SaveManager::worldMap() {
  return m_world_map;
}
map<string, pair<LevelData, UserData>> SaveManager::levelMap() {
  return m_level_map;
}