#ifndef STATE_H__
#define STATE_H__

#include <SFML\Graphics.hpp>

#include "../game/resource_manager.h"
#include "../game/Player.h"
#include "../game/music_player.h"
#include "../game/sound_player.h"

class StateStack;
class World;
class LevelManager;

class State {
public:
  typedef std::unique_ptr<State> Ptr;

  struct Context {
    Context(sf::RenderWindow &window, TextureManager &textureManager, FontManager &fontManager, LevelManager &levelManager, MusicPlayer &musicPlayer, SoundPlayer &soundPlayer, Player &player) :
      window(&window), textureManager(&textureManager), fontManager(&fontManager), levelManager(&levelManager), musicPlayer(&musicPlayer), soundPlayer(&soundPlayer), player(&player) {}

    sf::RenderWindow *window;
    TextureManager *textureManager;
    FontManager *fontManager;
    LevelManager *levelManager;
    MusicPlayer *musicPlayer;
    SoundPlayer *soundPlayer;
    Player *player;
  };

  enum ID {
    None,
    Title,
    Play,
    Pause,
    WorldSelect,
    LevelSelect,
    LevelCompletion,
    LoadingWorld,
    Loading,
    Option,
    Menu,
    Help
  };

public:
  State(StateStack &stack, Context context);
  virtual ~State();

  virtual void draw() = 0;
  virtual bool update(sf::Time deltaTime) = 0;
  virtual bool handleEvent(const sf::Event &event) = 0;
  virtual void onResolutionChange() = 0;

protected:
  void requestStackPush(ID stateID, const std::string &map = "level.json");
  void requestStackPop();
  void requestStackClear();
  void requestStackReload();
  void triggerResolutionChange();
  Context getContext() const;

private:
  StateStack *mStack;
  Context mContext;
};

#endif  // STATE_H__