#ifndef COLLISION_HANDLER_H__
#define COLLISION_HANDLER_H__

#include <Box2D\Box2D.h>

#include "../scene/scene_node.h"

class World;

class CollisionHandler : public b2ContactListener {
public:
  CollisionHandler(World &world);

  void BeginContact(b2Contact *contact);
  void PreSolve(b2Contact *contact, const b2Manifold *oldManifold);
  void EndContact(b2Contact *contact);

private:
  bool matchesCategories(SceneNode::Pair &nodes, unsigned int type1, unsigned int type2);

private:
  World &mWorld;
};

#endif  // COLLISION_HANDLER_H__