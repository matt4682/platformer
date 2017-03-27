#include "./level_completion_state.h"

#include <memory>
#include <iostream>

#include "../components/container.h"
#include "../components/Image.h"
#include "../components/Button.h"
#include "../components/Label.h"
#include "../components/Number.h"
#include "../components/buttons/container_button.h"
#include "../../save_manager.h"

using namespace std;

LevelCompletionState::LevelCompletionState(StateStack &stack, Context context) :
  State(stack, context),
  mGUIContainer(context, context.window->getDefaultView()) {
  sf::Vector2f windowSize = context.window->getDefaultView().getSize();
  LastCompletedData data = SaveManager::sharedInstance()->lastCompletedData();

  auto d_time = get<LastDataIndex::BeatTime>(data);
  auto d_deaths = get<LastDataIndex::Deaths>(data);
  auto d_level = get<LastDataIndex::MapName>(data);

  auto data_map = SaveManager::sharedInstance()->levelMap();
  auto level_data = data_map[d_level].first;

  auto d_badge_time = get<LevelDataIndex::TargetTime>(level_data);

  //Background panel where our stats will be displayed
  auto backgroundPanel = std::make_shared<GUI::Image>(
    getContext().textureManager->get(TextureID::LevelCompletionPanel));
  backgroundPanel->setPosition(windowSize.x / 2.f, windowSize.y / 2.f);
  mGUIContainer.add(backgroundPanel);

  //Stats
  GUI::Label::Ptr titleLabel;
  titleLabel = std::make_shared<GUI::Label>(
    " Level Complete!",
    sf::Color::White,
    36,
    *getContext().fontManager);

  GUI::Image::Ptr clock = std::make_shared<GUI::Image>(
    getContext().textureManager->get(TextureID::HUDSpriteSheet),
    sf::IntRect(48, 140, 48, 48));

  GUI::Number::Ptr time = std::make_shared<GUI::Number>(
    *getContext().textureManager, d_time);

  GUI::Image::Ptr badge = std::make_shared<GUI::Image>(
    getContext().textureManager->get(TextureID::HUDSpriteSheet),
    sf::IntRect(54, 49, 48, 48));

  GUI::Number::Ptr badgeTime = std::make_shared<GUI::Number>(
    *getContext().textureManager, d_badge_time);

  GUI::Image::Ptr heart = std::make_shared<GUI::Image>(
    getContext().textureManager->get(TextureID::HUDSpriteSheet),
    sf::IntRect(0, 94, 53, 45));

  GUI::Number::Ptr deaths = std::make_shared<GUI::Number>(
    *getContext().textureManager, d_deaths);

  mGUIContainer.add(titleLabel);
  mGUIContainer.add(clock);
  mGUIContainer.add(time);
  mGUIContainer.add(badge);
  mGUIContainer.add(badgeTime);
  mGUIContainer.add(deaths);
  mGUIContainer.add(heart);

  titleLabel->setPosition(
    windowSize.x / 2.f,
    windowSize.y / 2.f - 200.f);

  clock->setPosition(
    windowSize.x / 2.f - 100.f,
    windowSize.y / 2.f);

  time->setPosition(
    windowSize.x / 2.f + 100.f,
    windowSize.y / 2.f);

  badge->setPosition(
    windowSize.x / 2.f - 100.f,
    windowSize.y / 2.f + 100.f);

  badgeTime->setPosition(
    windowSize.x / 2.f + 100.f,
    windowSize.y / 2.f + 100.f);

  heart->setPosition(
    windowSize.x / 2.f - 100.f,
    windowSize.y / 2.f + 200.f);

  deaths->setPosition(
    windowSize.x / 2.f + 120.f,
    windowSize.y / 2.f + 200.f);
}

void LevelCompletionState::draw() {
  sf::RenderWindow &window = *getContext().window;
  window.setView(window.getDefaultView());
  window.draw(mGUIContainer);
}

bool LevelCompletionState::update(sf::Time deltaTime) {
  return false;
}

bool LevelCompletionState::handleEvent(const sf::Event &event) {
  if (event.type == sf::Event::KeyPressed) {
    if (event.key.code == sf::Keyboard::Escape || event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space) {
      requestStackClear();

      LastCompletedData data = SaveManager::sharedInstance()->lastCompletedData();

      auto map = get<LastDataIndex::WorldKey>(data);

      if (map.find("/") != std::string::npos)
        map = map.substr(0, map.find("/"));

      requestStackPush(ID::LevelSelect, map);

      //Stop music so main theme will play
      getContext().musicPlayer->stop();
    }
  }
  mGUIContainer.handleEvent(event);
  return false;
}

void LevelCompletionState::onResolutionChange() {}