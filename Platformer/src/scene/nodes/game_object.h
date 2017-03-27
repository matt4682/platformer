#pragma once

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>

#include "../scene_node.h"
#include "../command.h"
#include "../../game/tilemap/object.h"

class GameObject : public SceneNode {
public:
  typedef std::unique_ptr<GameObject> Ptr;

public:
  GameObject(Category::Type category, sf::Sprite sprite, b2Body *objectBody);

  unsigned int getCategory() const;
  virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
  virtual void updateCurrent(sf::Time deltaTime);
  virtual void remove();

private:
  Category::Type mCategory;
  sf::Sprite mSprite;
  b2Body *mBody;
};
