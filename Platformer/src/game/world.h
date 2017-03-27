#ifndef WORLD_H__
#define WORLD_H__

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>
#include <array>
#include <queue>

#include "./resource_manager.h"
#include "../scene/scene_node.h"
#include "./map_data.h"
#include "./tilemap/tile_loader.h"
#include "../scene/nodes/sprite_node.h"
#include "./tilemap/box2d_loader.h"
#include "../scene/factories/game_object_factory.h"
#include "./collision_handler.h"
#include "../scene/command.h"
#include "../scene/nodes/text_node.h"
#include "./sound_player.h"
#include "../ui/state.h"

class MainCharacter;

class World {
public:
  World(State::Context context, const std::string &map);

  CommandQueue &getCommandQueue();
  void initialize();
  void update(sf::Time deltaTime);
  void draw();
  void requestReset();
  void requestCompletion();
  bool isComplete();
  float getAttemptTime();
  int getNumDeaths();
  bool isGemCollected();
  void collectGem();
  void onResolutionChange();
  bool mapLoaded();
  bool objectsLoaded();
  bool texturesLoaded();

private:
  void buildScene();
  void spawnPlayer(sf::Vector2f position);
  void renderStaticBodyFixtures();
  void reset();
  void centerPlayerView();
  void updateSeekers();

private:
  enum Layer {
    Background,
    Object,
    Tilemap,
    HUD,
    Sounds,
    LayerCount
  };

private:
  State::Context mContext;
  sf::View mWorldView;
  CommandQueue mCommandQueue;
  TiledJSONLoader mMapLoader;
  Box2DLoader mWorldLoader;
  GameObjectFactory mGameObjectFactory;
  CollisionHandler mCollisionHandler;
  SceneNode mSceneGraph;
  std::array<SceneNode *, LayerCount> mSceneLayers;
  MainCharacter *mPlayerCharacter;
  b2Body *mPlayerBody;
  std::unique_ptr<b2World> mBox2DWorld;
  MapData mMapData;
  std::string mMap;
  sf::Time mLevelTimeElapsed;
  float mCompletionTime;
  TextNode *mTimeText;
  TextNode *mDeathText;
  int numDeaths;
  bool mGemCollected;
  bool mResetRequested;
  bool mCompletionRequested;
  bool mCompleted;
  bool mMapLoaded;
  bool mObjectsLoaded;
  bool mTexturesLoaded;
  SceneNode mBackgroundScene;
};

#endif  // WORLD_H__