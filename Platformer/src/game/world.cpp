#include "./World.h"

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>

#include <array>
#include <memory>
#include <iostream>
#include <sstream>
#include <string>

#include "../scene/nodes/entities/main_character.h"
#include "../scene/scene_node.h"
#include "../scene/nodes/text_node.h"
#include "../scene/nodes/tilemap_node.h"
#include "../scene/nodes/sound_node.h"
#include "./resource_manager.h"
#include "./tilemap/tile_loader.h"
#include "./tilemap/box2d_loader.h"
#include "./map_data.h"
#include "../scene/factories/game_object_factory.h"
#include "../scene/factories/entity_factory.h"
#include "./collision_handler.h"
#include "../save_manager.h"
#include "../log.h"
#include "../constants.h"

//#include "Bee.h"

using namespace std;

World::World(State::Context context, const std::string &map) :
  mContext(context),
  mWorldView(mContext.window->getDefaultView()),
  mMapLoader(TiledJSONLoader("res/Maps/", "res/Textures/Tileset/")),
  mWorldLoader(Box2DLoader()),
  mCollisionHandler(CollisionHandler(*this)),
  mSceneGraph(),
  mPlayerCharacter(nullptr),
  numDeaths(0),
  mGemCollected(false),
  mResetRequested(false),
  mCompletionRequested(false),
  mCompleted(false),
  mMapLoaded(false),
  mObjectsLoaded(false),
  mTexturesLoaded(false),
  mMap(map) {}

void World::initialize() {
  mMapLoader.load(mMap);
  mMapLoaded = mMapLoader.isMapLoaded();
  assert(mMapLoader.isMapLoaded());
  mMapData.tileWidth = mMapLoader.getTileSize().x;
  mMapData.tileHeight = mMapLoader.getTileSize().y;
  mMapData.mapWidth = mMapLoader.getMapSize().x;
  mMapData.mapHeight = mMapLoader.getMapSize().y;
  mMapData.tileLayers = mMapLoader.getTileLayers();
  mMapData.objectGroups = mMapLoader.getObjectGroups();

  mWorldLoader.load(mMapLoader.getTileLayers().at(1).tiles);
  mObjectsLoaded = mWorldLoader.isWorldLoaded();
  assert(mWorldLoader.isWorldLoaded());
  mBox2DWorld = std::unique_ptr<b2World>(mWorldLoader.getWorld());
  mBox2DWorld->SetContactListener(&mCollisionHandler);
  mGameObjectFactory = GameObjectFactory(mMapData, mBox2DWorld.get());
  buildScene();
  mTexturesLoaded = true;

  centerPlayerView();
  mContext.window->setView(mWorldView);
}

void World::reset() {
  mBox2DWorld.release();
  mLevelTimeElapsed = sf::Time::Zero;
  mCompletionTime = 0.0f;
  mPlayerCharacter = nullptr;
  mPlayerBody = nullptr;
  mWorldLoader.load(mMapData.tileLayers.at(1).tiles);
  assert(mWorldLoader.isWorldLoaded());
  mBox2DWorld = std::unique_ptr<b2World>(mWorldLoader.getWorld());
  mBox2DWorld->SetContactListener(&mCollisionHandler);
  mGameObjectFactory = GameObjectFactory(mMapData, mBox2DWorld.get());
  for (std::size_t i = 0; i < LayerCount; ++i) {
    mSceneGraph.detachChild(*mSceneLayers[i]);
  }
  mSceneLayers.empty();
  buildScene();
  centerPlayerView();
  mResetRequested = false;
  mGemCollected = false;
  ++numDeaths;
}

void World::requestReset() {
  mResetRequested = true;
  Command resetSoundCommand;
  resetSoundCommand.category = Category::SoundEffect;
  resetSoundCommand.action = [](SceneNode &node, sf::Time deltaTime) {
    SoundNode &sound = static_cast<SoundNode &>(node);
    sound.play(SoundEffectID::PlayerDie);
  };
  mCommandQueue.push(resetSoundCommand);
}

CommandQueue &World::getCommandQueue() {
  return mCommandQueue;
}

void World::update(sf::Time deltaTime) {
  if (!mCompletionRequested) {
    mLevelTimeElapsed += deltaTime;

    //It's important we advance our physics engine before updating
    mBox2DWorld->Step(deltaTime.asSeconds(), 6, 2);

    updateSeekers();
    while (!mCommandQueue.empty()) {
      mSceneGraph.onCommand(mCommandQueue.front(), deltaTime);
      mCommandQueue.pop();
    }

    if (mPlayerCharacter->getPosition().y > (mMapData.mapHeight * game::PX_PER_M)) {
      requestReset();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && mLevelTimeElapsed.asSeconds() > 1.f)
      requestReset();

    mSceneGraph.removeChildren();
    mSceneGraph.update(deltaTime);
    centerPlayerView();

    //Reposition and update our time
    std::ostringstream timeStream;
    timeStream << static_cast<int>(mLevelTimeElapsed.asSeconds() * 100.f) / 100.f;
    mTimeText->setText(timeStream.str());
    mTimeText->setPosition(
      mWorldView.getCenter().x + mWorldView.getSize().x / 2.f - 155.f,
      mWorldView.getCenter().y - mWorldView.getSize().y / 2.f + 25.f);
    std::ostringstream s;
    s << "Deaths: " << numDeaths;
    mDeathText->setText(s.str());
    mDeathText->setPosition(mWorldView.getCenter().x + mWorldView.getSize().x / 2.f - 155.f,
      mWorldView.getCenter().y - mWorldView.getSize().y / 2.f + 60.f);

    if (mResetRequested)
      reset();
  }

  else if (!mPlayerCharacter->isFaded()) {
    mPlayerCharacter->update(deltaTime);
  }

  else {
    mCompleted = true;
  }

  mContext.soundPlayer->removeStoppedSounds();
}

void World::updateSeekers() {
  /*Command seekCommand;
  seekCommand.category = Category::Seeker;
  sf::Vector2f playerPos = mPlayerCharacter->getRenderPosition();
  seekCommand.action = [playerPos](SceneNode &node, sf::Time deltaTime){
      Bee &bee = static_cast<Bee&>(node);
      bee.seek(playerPos);
  };
  mCommandQueue.push(seekCommand);*/
}

void World::requestCompletion() {
  mCompletionRequested = true;
  std::ostringstream timeStream;
  mCompletionTime = static_cast<int>(mLevelTimeElapsed.asSeconds() * 100.f) / 100.f;
  mPlayerCharacter->fade();
  Command fadeSoundCommand;
  fadeSoundCommand.category = Category::SoundEffect;
  fadeSoundCommand.action = [](SceneNode &node, sf::Time deltaTime) {
    SoundNode &sound = static_cast<SoundNode &>(node);
    sound.play(SoundEffectID::PlayerFade);
  };
  mCommandQueue.push(fadeSoundCommand);
}

bool World::isComplete() {
  return mCompleted;
}

float World::getAttemptTime() {
  return mCompletionTime;
}

int World::getNumDeaths() {
  return numDeaths;
}

bool World::isGemCollected() {
  return mGemCollected;
}

void World::collectGem() {
  Command gemSoundCommand;
  gemSoundCommand.category = Category::SoundEffect;
  gemSoundCommand.action = [](SceneNode &node, sf::Time deltaTime) {
    SoundNode &sound = static_cast<SoundNode &>(node);
    sound.play(SoundEffectID::GemPickup);
  };
  mCommandQueue.push(gemSoundCommand);
  gamelog(DEBUG) << "Got gem!" << std::endl;
  mGemCollected = true;
}

void World::centerPlayerView() {
  //Make sure our view is inside map bounds
  float viewWidth = mWorldView.getSize().x;
  float viewHeight = mWorldView.getSize().y;
  sf::Vector2f viewCenter = mWorldView.getCenter();
  sf::Vector2f playerPos = mPlayerCharacter->getRenderPosition();
  float mMapWidthPixels = mMapData.mapWidth * game::PX_PER_M;
  float mMapHeightPixels = mMapData.mapHeight * game::PX_PER_M;

  float xOffset = 0;
  float yOffset = 0;

  if (playerPos.y - viewHeight / 2 < 0)
    yOffset = fabs(playerPos.y - viewHeight / 2);
  else if (playerPos.y + viewHeight / 2 > mMapHeightPixels)
    yOffset = -1 * (viewHeight / 2 - (mMapHeightPixels - playerPos.y));
  if (playerPos.x - viewWidth / 2 < 0)
    xOffset = fabs(playerPos.x - viewWidth / 2);
  else if (playerPos.x + viewWidth / 2 > mMapWidthPixels)
    xOffset = -1 * (viewWidth / 2 - (mMapWidthPixels - playerPos.x));

  sf::Vector2f newCenter(playerPos.x + xOffset, playerPos.y + yOffset);
  mWorldView.setCenter(newCenter);
}

void World::draw() {
  mContext.window->draw(mBackgroundScene);
  mContext.window->setView(mWorldView);
  mContext.window->draw(mSceneGraph);
  renderStaticBodyFixtures();
}

void World::buildScene() {
  //Setup our layers
  for (std::size_t i = 0; i < LayerCount; ++i) {
    SceneNode::Ptr layer(new SceneNode());
    mSceneLayers[i] = layer.get();
    mSceneGraph.attachChild(std::move(layer));
  }
  gamelog(DEBUG) << "Dealing with background layer" << std::endl;

  //Background layer
  std::unique_ptr<SpriteNode> backgroundSprite;
  sf::Texture &cavetexture = mContext.textureManager->get(TextureID::CaveBackground);
  cavetexture.setRepeated(false);

  sf::Texture &texture = mContext.textureManager->get(TextureID::Background);
  texture.setRepeated(false);

  int map_width = static_cast<int>(mMapData.mapWidth * game::PX_PER_M);
  int map_height = static_cast<int>(mMapData.mapHeight * game::PX_PER_M);
  if (mMap.find("World2") == 0) {
    backgroundSprite.reset(new SpriteNode(cavetexture, sf::IntRect(0, 0, map_width, map_height)));
  }
  else {
    backgroundSprite.reset(new SpriteNode(texture, sf::IntRect(0, 0, map_width, map_height)));
  }

  mBackgroundScene.attachChild(std::move(backgroundSprite));

  gamelog(DEBUG) << "Dealing with tilemap layer" << std::endl;
  //Tilemap layer
  std::unique_ptr<TilemapNode> tileMap(new TilemapNode(mMapData, mWorldLoader.getContours()));
  mSceneLayers[Tilemap]->attachChild(std::move(tileMap));

  gamelog(DEBUG) << "Dealing with object layer" << std::endl;
  //Object layer (players, enemies, etc)
  EntityFactory entityFactory = EntityFactory(*mContext.textureManager, mMapData, mBox2DWorld.get());

  auto data_map = SaveManager::sharedInstance()->levelMap();

  for (auto &objectGroup : mMapData.objectGroups) {
    if (objectGroup.name == "Objects") {
      gamelog(DEBUG) << "Dealing with objects" << std::endl;
      for (auto &object : objectGroup.objects) {
        if (object.type == "Gem" && get<UserDataIndex::AcquiredGem>(data_map[mMap].second))
          continue;
        std::cout << get<UserDataIndex::AcquiredGem>(data_map[mMap].second) << std::endl;
        gamelog(DEBUG) << "Creating game object: " << object.type << std::endl;
        mSceneLayers[Object]->attachChild(GameObject::Ptr(mGameObjectFactory.createGameObject(object)));
      }
    }
    else if (objectGroup.name == "Spawns") {
      gamelog(DEBUG) << "Dealing with spawns" << std::endl;
      for (auto &object : objectGroup.objects) {
        if (object.type == "Player")
          spawnPlayer(object.position);
        else {
          mSceneLayers[Object]->attachChild(
            Entity::Ptr(entityFactory.createEntity(object)));
        }
      }
    }
  }
  gamelog(DEBUG) << "Dealing with player" << std::endl;
  mSceneLayers[Object]->attachChild(std::move(std::unique_ptr<MainCharacter>(mPlayerCharacter)));

  gamelog(DEBUG) << "Dealing with hud layer" << std::endl;
  //HUD Layer
  sf::Font &timeFont = mContext.fontManager->get(FontID::Thin);
  std::unique_ptr<TextNode> timeText(new TextNode(timeFont));
  std::unique_ptr<TextNode> deathText(new TextNode(timeFont));
  mTimeText = timeText.get();
  mTimeText->setColor(sf::Color::White);
  mDeathText = deathText.get();
  mDeathText->setColor(sf::Color::White);
  mSceneLayers[HUD]->attachChild(std::move(timeText));
  mSceneLayers[HUD]->attachChild(std::move(deathText));

  gamelog(DEBUG) << "Dealing with sound" << std::endl;
  //Sound Effects
  std::unique_ptr<SoundNode> soundEffects(new SoundNode(*mContext.soundPlayer));
  mSceneLayers[Sounds]->attachChild(std::move(soundEffects));
}

void World::spawnPlayer(sf::Vector2f position) {
  sf::Sprite playerSprite(mContext.textureManager->get(TextureID::PlayerStanding));
  sf::FloatRect bounds = playerSprite.getGlobalBounds();
  sf::Vector2f renderPos = position + sf::Vector2f(0.f, bounds.height / 2);
  sf::Vector2f centerPlayerPos = sf::Vector2f(
    renderPos.x,
    mMapData.mapHeight*game::PX_PER_M - renderPos.y);

  //Create our box2D body
  b2BodyDef playerDef;
  playerDef.fixedRotation = true;
  playerDef.type = b2_dynamicBody;
  playerDef.position.Set(centerPlayerPos.x / game::PX_PER_M, centerPlayerPos.y / game::PX_PER_M);
  mPlayerBody = mBox2DWorld->CreateBody(&playerDef);

  //Bounding box with no friction to prevent sticking to static body walls
  b2PolygonShape boundingBox;
  boundingBox.SetAsBox(bounds.width / game::PX_PER_M / 2 - 0.1f, bounds.height / game::PX_PER_M / 2);
  b2FixtureDef playerFixture;
  playerFixture.friction = 0.f;
  playerFixture.shape = &boundingBox;
  mPlayerBody->CreateFixture(&playerFixture);

  //Special foot sensor to manage jumping/movement
  b2PolygonShape footShape;
  b2Vec2 footVertices[4];
  footVertices[0] = b2Vec2(-bounds.width / game::PX_PER_M / 2 + 0.15f, -bounds.height / game::PX_PER_M / 2 - 0.04f);
  footVertices[1] = b2Vec2(-bounds.width / game::PX_PER_M / 2 + 0.15f, -bounds.height / game::PX_PER_M / 2 + 0.05f);
  footVertices[2] = b2Vec2(bounds.width / game::PX_PER_M / 2 - 0.15f, -bounds.height / game::PX_PER_M / 2 + 0.05f);
  footVertices[3] = b2Vec2(bounds.width / game::PX_PER_M / 2 - 0.15f, -bounds.height / game::PX_PER_M / 2 - 0.04f);
  footShape.Set(footVertices, 4);
  b2FixtureDef footFixture;
  footFixture.isSensor = true;
  footFixture.friction = 0.25f;
  footFixture.shape = &footShape;
  mPlayerBody->CreateFixture(&footFixture);

  mPlayerCharacter = new MainCharacter(*mContext.textureManager, mPlayerBody);
  mPlayerCharacter->setRenderPosition(renderPos);
  mPlayerBody->SetUserData(mPlayerCharacter);
}

void World::renderStaticBodyFixtures() {
  for (b2Body *b = mBox2DWorld->GetBodyList(); b; b = b->GetNext()) {
    if (static_cast<SceneNode *>(b->GetUserData())->getCategory() & Category::Type::Walkable) {
      for (b2Fixture *fixture = b->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
        b2ChainShape *chain = static_cast<b2ChainShape *>(fixture->GetShape());
        sf::VertexArray lines(sf::PrimitiveType::Lines);

        for (int i = 0; i < chain->GetChildCount(); ++i) {
          b2EdgeShape edge;
          chain->GetChildEdge(&edge, i);
          b2Vec2 v1 = edge.m_vertex1;
          b2Vec2 v2 = edge.m_vertex2;
          lines.append(sf::Vertex(sf::Vector2f(v1.x * game::PX_PER_M, (mMapData.mapHeight * game::PX_PER_M) - v1.y * game::PX_PER_M), sf::Color::Blue));
          lines.append(sf::Vertex(sf::Vector2f(v2.x * game::PX_PER_M, (mMapData.mapHeight * game::PX_PER_M) - v2.y * game::PX_PER_M), sf::Color::Blue));
        }
        mContext.window->draw(lines);
      }
    }
  }
}

void World::onResolutionChange() {
  mWorldView.setSize(sf::Vector2f(
    static_cast<float>(mContext.window->getSize().x),
    static_cast<float>(mContext.window->getSize().y)));
  centerPlayerView();
  mTimeText->setPosition(
    mWorldView.getCenter().x + mWorldView.getSize().x / 2.f - 125.f,
    mWorldView.getCenter().y - mWorldView.getSize().y / 2.f + 25.f);
}

bool World::mapLoaded() {
  return mMapLoaded;
}

bool World::objectsLoaded() {
  return mObjectsLoaded;
}

bool World::texturesLoaded() {
  return mTexturesLoaded;
}