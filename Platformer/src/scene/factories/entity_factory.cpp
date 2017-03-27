#include "./entity_factory.h"

#include <Box2D\Box2D.h>

#include <string>
#include <limits>
#include <cstdlib>

#include "../../game/resource_manager.h"
#include "../../game/map_data.h"
#include "../../game/tilemap/object.h"
#include "../nodes/entity.h"
#include "../command.h"
#include "../nodes/entities/jumper.h"
#include "../nodes/entities/jumping_block.h"
#include "../nodes/entities/moving_platform.h"
#include "../nodes/entities/big_jumper.h"
#include "../../constants.h"

EntityFactory::EntityFactory(TextureManager &textureManager, MapData data, b2World *world) :
  mTextureManager(textureManager), mMapData(data), mWorld(world) {}

Entity *EntityFactory::createEntity(Object &object) {
  std::string type = object.type;
  b2Body *objectBody = createPhysicsBody(object);

  Entity *newEntity;
  sf::Texture &spriteSheet = mTextureManager.get(TextureID::EnemiesSpriteSheet);
  sf::Texture &bossSlime = mTextureManager.get(TextureID::SlimeBoss);

  sf::Sprite enemySprite;

  if (type == "Big Jumper") {
    enemySprite.setTexture(bossSlime);
    enemySprite.setTextureRect(sf::IntRect(0, 214, 295, 186));
  }
  else
    enemySprite.setTexture(spriteSheet);

  if (type == "SnakeSlime") {
    enemySprite.setTextureRect(sf::IntRect(424, 187, 53, 147));
  }
  if (type == "Barnacle") {
    enemySprite.setTextureRect(sf::IntRect(318, 239, 51, 57));
  }
  if (type == "Jumper") {
    enemySprite.setTextureRect(sf::IntRect(140, 65, 49, 34));
  }
  if (type == "GrassBlock") {
    enemySprite.setTextureRect(sf::IntRect(0, 141, 71, 70));
  }
  if (type == "GrassPlatform") {
    enemySprite.setTextureRect(sf::IntRect(579, 272, 209, 39));
  }
  if (type == "GrassPlatformLarge") {
    enemySprite.setTextureRect(sf::IntRect(210, 0, 585, 39));
  }
  if (type == "Bee") {
    enemySprite.setTextureRect(sf::IntRect(315, 353, 56, 48));
  }
  sf::FloatRect bounds = enemySprite.getGlobalBounds();
  sf::Vector2f renderPos = object.position + sf::Vector2f(mMapData.tileWidth / 2.f, (mMapData.tileHeight - bounds.height / 2.f + 1.f));
  /*if (type.compare("SnakeSlime") == 0){
      newEntity = new SnakeSlime(
          mTextureManager, objectBody);
  }*/
  /* if (type.compare("Barnacle") == 0){
       bool flipped = false;
       if (object.properties.find("flipped") != object.properties.end()){
           std::string flippedJSON = object.properties["flipped"].get_str();
           json_spirit::mValue flippedValue;
           json_spirit::read_string(flippedJSON, flippedValue);
           flipped = flippedValue.get_bool();
           if (flipped)
               renderPos = renderPos - sf::Vector2f(0.f, (mMapData.tileHeight - bounds.height + 1.f));
       }
       newEntity = new Barnacle(
           mTextureManager, objectBody, flipped);
   }*/
  if (type.compare("Jumper") == 0) {
    std::string jmp = object.properties["jumpHeight"];
    newEntity = new Slime(mTextureManager, objectBody, static_cast<float>(atof(jmp.c_str())));
  }
  if (type.compare("GrassBlock") == 0) {
    std::string jmp = object.properties["jumpHeight"];
    newEntity = new GrassBlock(mTextureManager, objectBody, static_cast<float>(atof(jmp.c_str())));
  }
  if (type.compare("GrassPlatform") == 0 || type.compare("GrassPlatformLarge") == 0) {
    //Parse waypoints
    std::vector<sf::Vector2f> waypoints;
    nlohmann::json waypointsValue = object.properties["waypoints"];
    for (auto &wp : waypointsValue) {
      sf::Vector2f waypoint(
        wp["x"] * game::PX_PER_M,
        wp["y"] * game::PX_PER_M);
      waypoints.push_back(waypoint);
    }
    if (type.compare("GrassPlatform") == 0) {
      std::string vel = object.properties["velocity"];
      newEntity = new GrassPlatform(mTextureManager, objectBody, static_cast<float>(atof(vel.c_str())), waypoints);
    }
    else {
      std::string vel = object.properties["velocity"];
      newEntity = new GrassPlatform(mTextureManager, objectBody, static_cast<float>(atof(vel.c_str())), waypoints, true);
    }
  }
  if (type.compare("Bee") == 0) {
    //Parse waypoints
    std::vector<sf::Vector2f> waypoints;
    if (object.properties.find("waypoints") != object.properties.end()) {
      nlohmann::json waypointsValue = object.properties["waypoints"];
      for (auto &wp : waypointsValue) {
        sf::Vector2f waypoint(
          wp["x"] * game::PX_PER_M,
          wp["y"] * game::PX_PER_M);
        waypoints.push_back(waypoint);
      }
    }
    /* if (object.properties.find("steering") != object.properties.end())
         newEntity = new Bee(
             mTextureManager, objectBody, atof(object.properties["velocity"].get_str().c_str()), waypoints, atof(object.properties["steering"].get_str().c_str()));

     else
         newEntity = new Bee(
             mTextureManager, objectBody, atof(object.properties["velocity"].get_str().c_str()), waypoints);*/
  }

  if (type.compare("Big Jumper") == 0) {
    std::string jmp = object.properties["jumpHeight"];
    newEntity = new BossSlime(mTextureManager, objectBody, static_cast<float>(atof(jmp.c_str())));
  }

  newEntity->setRenderPosition(renderPos);
  objectBody->SetUserData(newEntity);
  return newEntity;
}

b2Body *EntityFactory::createPhysicsBody(Object &object) {
  std::string type = object.type;
  int mapHeightPixels = mMapData.mapHeight * mMapData.tileHeight;

  sf::Texture &spriteSheet = mTextureManager.get(TextureID::EnemiesSpriteSheet);
  sf::Texture &bossSlime = mTextureManager.get(TextureID::SlimeBoss);

  sf::Sprite enemySprite(spriteSheet);
  sf::FloatRect bounds;
  sf::Vector2f renderPos;

  if (type == "BossSlime") {
    enemySprite.setTexture(bossSlime);
    enemySprite.setTextureRect(sf::IntRect(0, 214, 295, 186));
    bounds = enemySprite.getGlobalBounds();
    renderPos = object.position + sf::Vector2f(mMapData.tileWidth / 2.f, (mMapData.tileHeight - 1 - bounds.height / 2.f));
  }
  if (type == "SnakeSlime") {
    enemySprite.setTextureRect(sf::IntRect(424, 187, 53, 147));
    bounds = enemySprite.getGlobalBounds();
    renderPos = object.position + sf::Vector2f(mMapData.tileWidth / 2.f, (mMapData.tileHeight - bounds.height / 2.f));
  }
  if (type == "Barnacle") {
    bool flipped = false;
    /*if(object.properties.find("flipped") != object.properties.end()) {
      std::string flippedJSON = object.properties["flipped"].get_str();
      json_spirit::mValue flippedValue;
      json_spirit::read_string(flippedJSON, flippedValue);
      flipped = flippedValue.get_bool();
    }*/
    enemySprite.setTextureRect(sf::IntRect(318, 239, 51, 57));
    bounds = enemySprite.getGlobalBounds();
    renderPos = object.position + sf::Vector2f(mMapData.tileWidth / 2.f, (mMapData.tileHeight - bounds.height / 2.f));
    if (flipped) {
      enemySprite.move(0.f, -(mMapData.tileHeight - enemySprite.getGlobalBounds().height));
      renderPos = object.position + sf::Vector2f(mMapData.tileWidth / 2.f, 0.f);
    }
  }
  if (type == "Jumper") {
    enemySprite.setTextureRect(sf::IntRect(140, 65, 49, 34));
    bounds = enemySprite.getGlobalBounds();
    renderPos = object.position + sf::Vector2f(mMapData.tileWidth / 2.f, (mMapData.tileHeight - bounds.height / 2.f));
  }
  if (type == "GrassBlock") {
    enemySprite.setTextureRect(sf::IntRect(0, 141, 71, 70));
    bounds = enemySprite.getGlobalBounds();
    renderPos = object.position + sf::Vector2f(mMapData.tileWidth / 2.f, (mMapData.tileHeight - bounds.height / 2.f));
  }
  if (type == "GrassPlatform") {
    enemySprite.setTextureRect(sf::IntRect(579, 272, 209, 39));
    bounds = enemySprite.getGlobalBounds();
    renderPos = object.position + sf::Vector2f(mMapData.tileWidth / 2.f, (mMapData.tileHeight - bounds.height / 2.f));
  }
  if (type == "GrassPlatformLarge") {
    enemySprite.setTextureRect(sf::IntRect(210, 0, 585, 39));
    bounds = enemySprite.getGlobalBounds();
    renderPos = object.position + sf::Vector2f(mMapData.tileWidth / 2.f, (mMapData.tileHeight - bounds.height / 2.f));
  }
  if (type == "Bee") {
    enemySprite.setTextureRect(sf::IntRect(315, 353, 56, 48));
    bounds = enemySprite.getGlobalBounds();
    renderPos = object.position + sf::Vector2f(mMapData.tileWidth / 2.f, (mMapData.tileHeight - bounds.height / 2.f));
  }
  sf::Vector2f centerEntityPos = sf::Vector2f(
    renderPos.x,
    mMapData.mapHeight*game::PX_PER_M - renderPos.y);

  b2BodyDef entityDef;
  entityDef.fixedRotation = true;
  if (type == "GrassBlock" || type == "Jumper" || type == "Big Jumper")
    entityDef.type = b2_dynamicBody;
  else
    entityDef.type = b2_kinematicBody;
  entityDef.position.Set(centerEntityPos.x / game::PX_PER_M, centerEntityPos.y / game::PX_PER_M);
  b2Body *entityBody = mWorld->CreateBody(&entityDef);

  //Bounding box with no friction to prevent sticking to static body walls
  b2PolygonShape boundingBox;
  boundingBox.SetAsBox(bounds.width / game::PX_PER_M / 2 - 0.1f, bounds.height / game::PX_PER_M / 2);
  b2FixtureDef playerFixture;
  playerFixture.friction = 1.f;
  playerFixture.restitution = 0.f;
  playerFixture.shape = &boundingBox;
  entityBody->CreateFixture(&playerFixture);

  //Bottom sensor for GrassBlock
  if (type == "GrassBlock") {
    b2PolygonShape blockSensor;
    b2Vec2 footVertices[4];
    footVertices[0] = b2Vec2(-bounds.width / game::PX_PER_M / 2 + 0.2f, -bounds.height / game::PX_PER_M / 2 - 0.01f);
    footVertices[1] = b2Vec2(-bounds.width / game::PX_PER_M / 2 + 0.2f, -bounds.height / game::PX_PER_M / 2 + 0.05f);
    footVertices[2] = b2Vec2(bounds.width / game::PX_PER_M / 2 - 0.2f, -bounds.height / game::PX_PER_M / 2 + 0.05f);
    footVertices[3] = b2Vec2(bounds.width / game::PX_PER_M / 2 - 0.2f, -bounds.height / game::PX_PER_M / 2 - 0.04f);
    blockSensor.Set(footVertices, 4);
    b2FixtureDef footFixture;
    footFixture.isSensor = true;
    footFixture.friction = 0.25f;
    footFixture.shape = &blockSensor;
    entityBody->CreateFixture(&footFixture);
    b2MassData grassMassData;
    entityBody->GetMassData(&grassMassData);
    grassMassData.mass = static_cast<float>(UINT_MAX);
    entityBody->SetMassData(&grassMassData);
  }
  return entityBody;
}