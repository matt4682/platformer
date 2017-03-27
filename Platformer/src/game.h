#ifndef GAME_H__
#define GAME_H__

#include <SFML/Graphics.hpp>

#include <string>

#include "./constants.h"
#include "./game/player.h"
#include "./game/resource_manager.h"
#include "./game/level_manager.h"
#include "./game/state_stack.h"
#include "./game/music_player.h"

class Game {
public:
  Game();

  void run();

private:
  void exit();
  void handleInput();
  void draw();
  void loadSaveFile();

private:
  sf::RenderWindow m_window{
    sf::VideoMode(game::WIDTH, game::HEIGHT),
    std::string(game::GAME_NAME) + " " + std::string(game::GAME_VERSION),
    sf::Style::Default
  };

  Player m_player{};

  TextureManager m_texture_manager{};
  FontManager m_font_manager{};
  LevelManager m_level_manager{};

  MusicPlayer m_music_player{};
  SoundPlayer m_sound_player{};

  StateStack m_state_stack{
    State::Context(
      m_window,
      m_texture_manager,
      m_font_manager,
      m_level_manager,
      m_music_player,
      m_sound_player,
      m_player
    )
  };
};

#endif  // GAME_H__