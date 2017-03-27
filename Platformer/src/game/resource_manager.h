#ifndef RESOURCE_MANAGER_H__
#define RESOURCE_MANAGER_H__

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <map>
#include <memory>
#include <string>
#include <cassert>
#include <iostream>

#include "../log.h"

class World;

enum TextureID {
  Background,
  CastleBackground,
  CaveBackground,
  GrasslandsBackground,
  PlayerSpriteSheet,
  EnemiesSpriteSheet,
  SlimeBoss,
  SpinnerSpriteSheet,
  HUDSpriteSheet,
  GUISpriteSheet,
  PlayerStanding,
  PauseScreenPanel,
  OptionScreenPanel,
  HelpScreenPanel,
  LevelCompletionPanel,
  LevelSelectionPanel,
  MenuGround,
  SelectedWorld
};

enum FontID {
  Main,
  Thin
};

enum SoundEffectID {
  PlayerDie,
  PlayerFade,
  Switch,
  GemPickup
};

template <typename Identifier, typename Resource>
class ResourceManager {
public:
  void load(Identifier id, const std::string &file);
  Resource &get(Identifier id);

private:
  std::map<Identifier, std::unique_ptr<Resource>> mResourceMap;
};

typedef ResourceManager<TextureID, sf::Texture> TextureManager;
typedef ResourceManager<FontID, sf::Font> FontManager;
typedef ResourceManager<SoundEffectID, sf::SoundBuffer> SoundBufferManager;

template <typename Identifier, typename Resource>
void ResourceManager<Identifier, Resource>::load(Identifier id, const std::string &file) {
  std::unique_ptr<Resource> resource(new Resource());

  if (!resource->loadFromFile(file)) {
    gamelog(ERROR) << "Error loading: " << file << std::endl;
  }

  mResourceMap[id] = std::move(resource);
}

template <typename Identifier, typename Resource>
Resource &ResourceManager<Identifier, Resource>::get(Identifier id) {
  auto resource = mResourceMap.find(id);
  assert(resource != mResourceMap.end());
  return *resource->second;
}

#endif  // RESOURCE_MANAGER_H__