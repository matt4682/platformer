#ifndef SPRITE_NODE_H__
#define SPRITE_NODE_H__

#include <SFML\Graphics.hpp>

#include "../scene_node.h"

class SpriteNode : public SceneNode {
public:
  explicit SpriteNode(const sf::Texture &texture);
  SpriteNode(const sf::Texture &texture, const sf::IntRect &rect);

private:
  virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

private:
  sf::Sprite mSprite;
};

#endif  // SPRITE_NODE_H__