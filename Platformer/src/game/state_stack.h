#ifndef STATE_STACK_H__
#define STATE_STACK_H__

#include "../ui/state.h"

class StateStack {
public:
  enum Action {
    Push,
    Pop,
    Clear,
    Resolution
  };

public:
  StateStack(State::Context context);

  void update(sf::Time deltaTime);
  void draw();
  void handleEvent(const sf::Event &event);
  State::Ptr createState(State::ID stateID, const std::string &map = "level.json");
  void pushState(State::ID stateID, const std::string &map = "level.json");
  void popState();
  void clearStates();
  void onResolutionChange();
  bool isEmpty() const;
  State::Context getContext() const;

private:
  void applyPendingChanges();

private:
  struct PendingChange {
    PendingChange(StateStack::Action action, State::ID stateID = State::ID::None, const std::string &map = "level.json") : action(action), stateID(stateID), map(map) {}

    StateStack::Action action;
    State::ID stateID;
    std::string map;
  };

private:
  std::vector<State::Ptr> mStack;
  std::vector<PendingChange> mPendingList;
  State::Context mContext;
};

#endif  // STATE_STACK_H__
