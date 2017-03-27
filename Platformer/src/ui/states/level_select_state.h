#ifndef LEVEL_SELECT_STATE_H__
#define LEVEL_SELECT_STATE_H__

#include "../state.h"
#include "../../game/world.h"
#include "../components/container.h"
#include "../../save_manager.h"

class StateStack;

class LevelSelectState : public State {
public:
  LevelSelectState(StateStack &stack, Context context, std::string world);

  void draw();
  bool update(sf::Time deltaTime);
  bool handleEvent(const sf::Event &event);
  void onResolutionChange();

private:
  void addLevel(std::pair<LevelData, UserData> lData, const std::string &map, bool enabled = true);
  void buildLevelPanel();
  int getCollectedGems();

private:
  GUI::Container mGUIContainer;
  GUI::Container::Ptr mLevelContainer;
  sf::Sprite mLevelPanel;
  sf::Sprite mBackground;
  sf::Sprite mBackdrop;
  unsigned int numLevels;
  std::string world;
};

#endif  // LEVEL_SELECT_STATE_H__