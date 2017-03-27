#ifndef PLAY_STATE_H__
#define PLAY_STATE_H__

#include "../state.h"
#include "../../game/world.h"
#include "../../game/player.h"
class StateStack;

class PlayState : public State {
public:
  PlayState(StateStack &stack, Context context, const std::string &map);

  void draw();
  bool update(sf::Time deltaTime);
  bool handleEvent(const sf::Event &event);
  void onResolutionChange();

private:
  std::unique_ptr<World> mWorld;
  Player &mPlayer;
  std::string mMapName;
};

#endif  // PLAY_STATE_H__