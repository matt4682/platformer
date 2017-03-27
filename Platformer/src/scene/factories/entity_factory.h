#pragma once

#include <Box2D\Box2D.h>

#include "../../game/resource_manager.h"
#include "../../game/tilemap/object.h"
#include "../nodes/game_object.h"
#include "../nodes/entity.h"
#include "../command.h"
#include "../../game/map_data.h"

class EntityFactory {
public:
  EntityFactory(TextureManager &textureManager, MapData data, b2World *world);

  Entity *createEntity(Object &object);

private:
  b2Body *createPhysicsBody(Object &object);
  MapData mMapData;
  b2World *mWorld;
  TextureManager &mTextureManager;
};
