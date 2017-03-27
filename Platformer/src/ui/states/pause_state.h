#ifndef PAUSE_STATE_H__
#define PAUSE_STATE_H__

#include "../state.h"
#include "../../game/world.h"
#include "../components/container.h"

class StateStack;

class PauseState : public State {
public:
  PauseState(StateStack &stack, Context context, std::string world);

  void draw();
  bool update(sf::Time deltaTime);
  bool handleEvent(const sf::Event &event);
  void onResolutionChange();

private:
  GUI::Container mGUIContainer;
  std::string mWorldName;
};

#endif  // PAUSE_STATE_H__