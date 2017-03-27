#ifndef OPTION_STATE_H__
#define OPTION_STATE_H__

#include "../state.h"
#include "../../game/world.h"
#include "../components/container.h"
#include "../components/option_container.h"

class StateStack;

class OptionState : public State {
public:
  OptionState(StateStack &stack, Context context);

  void draw();
  bool update(sf::Time deltaTime);
  bool handleEvent(const sf::Event &event);
  void applyOptions();
  void onResolutionChange();

private:
  GUI::Container mGUIContainer;
  GUI::OptionContainer::Ptr mOptionContainer;
};

#endif  // OPTION_STATE_H__