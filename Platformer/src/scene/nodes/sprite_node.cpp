#include "./sprite_node.h"

#include <SFML\Graphics.hpp>

SpriteNode::SpriteNode(const sf::Texture &texture) {
  mSprite.setTexture(texture);
}

SpriteNode::SpriteNode(const sf::Texture &texture, const sf::IntRect &rect) {
  mSprite.setTexture(texture);
  mSprite.setTextureRect(rect);
}

void SpriteNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(mSprite, states);
}