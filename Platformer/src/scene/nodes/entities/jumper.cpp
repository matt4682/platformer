#include "./jumper.h"

#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>
#include <cmath>

#include "../../../game/resource_manager.h"
#include "../../command.h"
#include "../../../constants.h"

Slime::Slime(TextureManager &textureManager, b2Body *enemyBody, float jumpHeight) :
  Entity(enemyBody),
  mBouncing(false),
  jumpVelocity(sqrt(fabs(2 * physics::GRAVITY *jumpHeight))) {
  sf::Texture &spriteSheet = textureManager.get(TextureID::EnemiesSpriteSheet);

  Animation bounceAnimation;
  bounceAnimation.setSpriteSheet(spriteSheet);
  bounceAnimation.addFrame(sf::IntRect(140, 65, 49, 34));
  bounceAnimation.addFrame(sf::IntRect(578, 312, 57, 34));
  bounceAnimation.addFrame(sf::IntRect(576, 457, 57, 34));
  bounceAnimation.addFrame(sf::IntRect(578, 312, 57, 34));
  mAnimation = bounceAnimation;

  mSprite = AnimatedSprite(sf::seconds(0.035f));
  mSprite.setAnimation(mAnimation);
  stopAnimation();
  sf::FloatRect bounds = mSprite.getLocalBounds();
  mSprite.setOrigin(bounds.width / 2, bounds.height / 2);
}

unsigned int Slime::getCategory() const {
  return Category::Enemy;
}

void Slime::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(mSprite, states);
}

void Slime::updateCurrent(sf::Time deltaTime) {
  b2Vec2 physPos = mB2Body->GetPosition();
  sf::Vector2f previousRenderPos = mSprite.getPosition();
  float mapHeight = previousRenderPos.y + mPreviousPosition.y * game::PX_PER_M;
  mSprite.setPosition(physPos.x * game::PX_PER_M, mapHeight - physPos.y * game::PX_PER_M);
  mPreviousPosition = physPos;
  mSprite.update(deltaTime);
  if (getVelocity().y == 0 && !mBouncing) {
    mSprite.play();
    mSprite.setLooped(false);
    mBouncing = true;
  }
  if (getVelocity().y == 0 && !mSprite.isPlaying()) {
    jump();
    mBouncing = false;
  }
}

void Slime::jump() {
  setVelocity(b2Vec2(getVelocity().x, jumpVelocity));
}

void Slime::stopAnimation() {
  if ((mSprite.getAnimation()->getSize() > 0) &&
    (mSprite.getAnimation()->getSize() < 10))
    mSprite.stop();
}