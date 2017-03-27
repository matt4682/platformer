#ifndef HELP_STATE_H__
#define HELP_STATE_H__

#include "../state.h"
#include "../../game/world.h"
#include "../components/container.h"

class StateStack;

class HelpState : public State {
public:
  HelpState(StateStack &stack, Context context);
  void draw();
  bool update(sf::Time deltaTime);
  bool handleEvent(const sf::Event &event);
  void onResolutionChange();

private:
  GUI::Container mGUIContainer;
};

#endif  // HELP_STATE_H__