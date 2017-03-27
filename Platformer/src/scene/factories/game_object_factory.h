#ifndef GAME_OBJECT_FACTORY_H__
#define GAME_OBJECT_FACTORY_H__

#include <Box2D\Box2D.h>

#include "../../game/tilemap/object.h"
#include "../nodes/game_object.h"
#include "../command.h"
#include "../../game/map_data.h"

class GameObjectFactory {
public:
  GameObjectFactory();
  GameObjectFactory(MapData data, b2World *world);

  GameObject *createGameObject(Object &object);

private:
  b2Body *createPhysicsBody(Object &object);
  b2Vec2 getObjectSize(Object &object);
  MapData mMapData;
  b2World *mWorld;
};

#endif  // GAME_OBJECT_FACTORY_H__
