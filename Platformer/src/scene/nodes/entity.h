#ifndef ENTITY_H__
#define ENTITY_H__

#include <Box2D\Box2D.h>

#include "../../game/resource_manager.h"
#include "../animated_sprite.h"
#include "../scene_node.h"

class Entity : public SceneNode {
public:
  enum FacingDirection {
    Left,
    Right
  };

  typedef std::unique_ptr<Entity> Ptr;

public:
  explicit Entity(b2Body *entityBody);

  sf::Vector2f getRenderPosition() const;
  void setRenderPosition(sf::Vector2f position);
  b2Vec2 getPosition() const;
  void setVelocity(b2Vec2 velocity);
  b2Vec2 getVelocity() const;
  sf::FloatRect getBoundingBox() const;

protected:
  b2Vec2 mVelocity;
  b2Vec2 mPreviousPosition;
  AnimatedSprite mSprite;
  b2Body *mB2Body;
};

#endif  // ENTITY_H__