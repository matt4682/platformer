#pragma once

#include "../state.h"
#include "../../game/world.h"
#include "../components/container.h"
#include "../../scene/animated_sprite.h"
#include "../../scene/animation.h"

class StateStack;

class MenuState : public State {
public:
  MenuState(StateStack &stack, Context context);

  void draw();
  bool update(sf::Time deltaTime);
  bool handleEvent(const sf::Event &event);
  void updateBackground();
  void onResolutionChange();

private:
  GUI::Container mGUIContainer;
  sf::Text mTitleText;
  sf::Sprite mGrass;
  sf::Sprite mBackground;
  sf::Sprite mBackdrop;
  Animation mCharacterAnimation;
  AnimatedSprite mCharacter;
};
