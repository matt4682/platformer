#include <SFML\Graphics.hpp>
#include "../../scene/command.h"
#include "./game_object.h"

GameObject::GameObject(Category::Type category, sf::Sprite sprite, b2Body *objectBody) :
  mCategory(category), mSprite(sprite), mBody(objectBody) {}

unsigned int GameObject::getCategory() const {
  return mCategory;
}

void GameObject::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
  auto boundingBox = static_cast<b2PolygonShape *>(mBody->GetFixtureList()->GetShape());
  target.draw(mSprite, states);
}

void GameObject::updateCurrent(sf::Time deltaTime) {}

void GameObject::remove() {
  mBody->GetWorld()->DestroyBody(mBody);
}