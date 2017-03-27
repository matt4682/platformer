#include "./Game.h"

#include <fstream>

#include "../lib/json/src/json.hpp"
#include "./save_manager.h"

using json = nlohmann::json;
using namespace std;

Game::Game() {
  // TODO(Matt): These should all be settings eventually
  m_window.setVerticalSyncEnabled(true);
  m_window.setFramerateLimit(game::FRAMERATE);
  m_window.setSize(sf::Vector2u(1280, 720));
  // m_window.setIcon(sfml_icon.width,  sfml_icon.height,  sfml_icon.pixel_data);

  // Load necessary textures and fonts
  // TODO(Matt): Go through these and remove / customize them
  m_texture_manager.load(TextureID::Background, "res/Textures/Background/bg.png");
  m_texture_manager.load(TextureID::CaveBackground, "res/Textures/Background/bgcave.png");
  m_texture_manager.load(TextureID::MenuGround, "res/Textures/Tileset/ground_repeat_texture.png");
  m_texture_manager.load(TextureID::GrasslandsBackground, "res/Textures/Background/bg_grasslands.png");
  m_texture_manager.load(TextureID::PauseScreenPanel, "res/Textures/GUI/grey_panel.png");
  m_texture_manager.load(TextureID::OptionScreenPanel, "res/Textures/GUI/grey_panel_medium2.png");
  m_texture_manager.load(TextureID::LevelCompletionPanel, "res/Textures/GUI/grey_panel_medium.png");
  m_texture_manager.load(TextureID::LevelSelectionPanel, "res/Textures/GUI/grey_panel_large.png");
  m_texture_manager.load(TextureID::PlayerSpriteSheet, "res/Textures/Player/player_spritesheet.png");
  m_texture_manager.load(TextureID::PlayerStanding, "res/Textures/Player/player_stand.png");
  m_texture_manager.load(TextureID::EnemiesSpriteSheet, "res/Textures/Enemy/enemies_spritesheet.png");
  m_texture_manager.load(TextureID::SpinnerSpriteSheet, "res/Textures/Enemy/spinner_spritesheet.png");
  m_texture_manager.load(TextureID::SlimeBoss, "res/Textures/Enemy/giant_slime.png");
  m_texture_manager.load(TextureID::GUISpriteSheet, "res/Textures/GUI/gui_spritesheet.png");
  m_texture_manager.load(TextureID::HUDSpriteSheet, "res/Textures/HUD/hud_spritesheet.png");

  m_texture_manager.load(TextureID::SelectedWorld, "res/Textures/GUI/selected_world.png");
  m_font_manager.load(FontID::Main, "res/Fonts/OpenSans.ttf");
  m_font_manager.load(FontID::Thin, "res/Fonts/OpenSans-Light.ttf");
  m_state_stack.pushState(State::ID::Menu);

  // Load game data
  loadSaveFile();
}

void Game::run() {
  sf::Time time_elapsed = sf::Time::Zero;
  sf::Clock clock;

  while (m_window.isOpen()) {
    time_elapsed += clock.restart();

    while (time_elapsed > game::FRAME_RATE) {
      time_elapsed -= game::FRAME_RATE;
      handleInput();

      m_state_stack.update(game::FRAME_RATE);

      if (m_state_stack.isEmpty()) {
        exit();
      }
    }
    draw();
  }
}

void Game::exit() {
  m_window.close();
}

void Game::handleInput() {
  sf::Event event;

  while (m_window.pollEvent(event)) {
    m_state_stack.handleEvent(event);
    if (event.type == sf::Event::Closed) {
      exit();
    }
  }
}

void Game::draw() {
  m_window.clear();
  m_state_stack.draw();
  m_window.display();
  m_window.setView(m_window.getDefaultView());
}

// TODO(Matt): Let SaveManager do this
void Game::loadSaveFile() {
  constexpr char *save_file = "save.dat";

  std::ifstream ifs(save_file);
  if (ifs.good()) {
    stringstream buffer;
    buffer << ifs.rdbuf();
    string data = buffer.str();
    try {
      auto save_data = json::parse(buffer.str());
      SaveManager::sharedInstance()->load(save_data);
    }
    catch (exception e) {
      cout << e.what() << endl;
    }
  }
  else {
    std::ofstream ofs(save_file);
    stringstream buffer;
    buffer << SaveManager::sharedInstance()->save();

    ofs << buffer.str();
  }
}