#ifndef PLAYER_H__
#define PLAYER_H__

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>

#include "../scene/command.h"

class Player {
public:
  void handleEvent(const sf::Event &event, CommandQueue &queue);
  void handleInput(CommandQueue &queue);
};

#endif  // PLAYER_H__