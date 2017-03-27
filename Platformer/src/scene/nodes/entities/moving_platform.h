#ifndef MOVING_PLATFORM_H__
#define MOVING_PLATFORM_H__

#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include <map>
#include <vector>

#include "../entity.h"
#include "../../../game/resource_manager.h"
#include "../../animated_sprite.h"

class GrassPlatform : public Entity {
public:
  GrassPlatform(TextureManager &textureManager, b2Body *enemyBody, float moveVelocity, std::vector<sf::Vector2f> waypoints, bool large = false);

  unsigned int getCategory() const;
  virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
  virtual void updateCurrent(sf::Time deltaTime);
  void activate();
  void deactivate();

private:
  std::vector<sf::Vector2f> mWaypoints;
  std::vector<sf::Vector2f>::iterator mCurrentWaypoint;
  Animation mAnimation;
  bool mMoving;
  float mMoveVelocity;
};

#endif  // MOVING_PLATFORM_H__
