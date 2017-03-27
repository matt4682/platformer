#ifndef WORLD_SELECT_STATE_H__
#define WORLD_SELECT_STATE_H__

#include "../state.h"
#include "../../game/world.h"
#include "../components/container.h"
#include "../../save_manager.h"

class StateStack;

class WorldSelectState : public State {
public:
  WorldSelectState(StateStack &stack, Context context, std::string world);

  void draw();
  bool update(sf::Time deltaTime);
  bool handleEvent(const sf::Event &event);
  void onResolutionChange();

private:
  void addWorld(const std::string &world, bool enabled = true);
  void buildWorldPanel();
  int getCollectedGems();

private:
  GUI::Container mGUIContainer;
  GUI::Container::Ptr mWorldContainer;
  sf::Sprite mWorldPanel;
  sf::Sprite mBackground;
  sf::Sprite mBackdrop;
  unsigned int numWorlds;
  std::string world;
  unsigned int defaultSelection;
};

#endif  // WORLD_SELECT_STATE_H__