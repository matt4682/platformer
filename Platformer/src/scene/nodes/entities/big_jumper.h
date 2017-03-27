#ifndef BIG_JUMPER_H__
#define BIG_JUMPER_H__

#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include <map>

#include "../entity.h"
#include "../../../game/resource_manager.h"
#include "../../animated_sprite.h"

class BossSlime : public Entity {
public:
  BossSlime(TextureManager &textureManager, b2Body *enemyBody, float jumpHeight);

  unsigned int getCategory() const;
  virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
  virtual void updateCurrent(sf::Time deltaTime);
  void jump();
  void stopAnimation();

private:
  Animation mAnimation;
  float jumpVelocity;
  bool mBouncing;
};

#endif  // BIG_JUMPER_H__
