#include "./big_jumper.h"

#include <SFML/Graphics.hpp>
#include <Box2D\Box2D.h>

#include <cmath>

#include "../../../game/resource_manager.h"
#include "../../command.h"
#include "../../../constants.h"

BossSlime::BossSlime(TextureManager &textureManager, b2Body *enemyBody, float jumpHeight) :
  Entity(enemyBody),
  mBouncing(false),
  jumpVelocity(sqrt(fabs(2 * physics::GRAVITY *jumpHeight))) {
  sf::Texture &spriteSheet = textureManager.get(TextureID::SlimeBoss);

  Animation walkAnimation;
  walkAnimation.setSpriteSheet(spriteSheet);
  walkAnimation.addFrame(sf::IntRect(0, 0, 295, 186));
  walkAnimation.addFrame(sf::IntRect(0, 214, 295, 186));
  mAnimation = walkAnimation;

  mSprite = AnimatedSprite(sf::seconds(0.25f));
  mSprite.setAnimation(mAnimation);
  stopAnimation();
  sf::FloatRect bounds = mSprite.getLocalBounds();
  mSprite.setOrigin(bounds.width / 2, bounds.height / 2);

  mSprite.setScale(1.f, 1.f);
}

unsigned int BossSlime::getCategory() const {
  return Category::Enemy;
}

void BossSlime::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(mSprite, states);
}

void BossSlime::updateCurrent(sf::Time deltaTime) {
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

void BossSlime::jump() {
  setVelocity(b2Vec2(getVelocity().x, jumpVelocity));
}

void BossSlime::stopAnimation() {
  if ((mSprite.getAnimation()->getSize() > 0) &&
    (mSprite.getAnimation()->getSize() < 10))
    mSprite.stop();
}