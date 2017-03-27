#ifndef LEVEL_COMPLETION_STATE_H__
#define LEVEL_COMPLETION_STATE_H__

#include "../state.h"
#include "../../game/world.h"
#include "../components/container.h"
#include "../../save_manager.h"

class StateStack;

class LevelCompletionState : public State {
public:
  LevelCompletionState(StateStack &stack, Context context);
  void draw();
  bool update(sf::Time deltaTime);
  bool handleEvent(const sf::Event &event);
  void onResolutionChange();

private:
  GUI::Container mGUIContainer;
};

#endif  // LEVEL_COMPLETION_STATE_H__