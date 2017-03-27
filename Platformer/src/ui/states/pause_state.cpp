#include "./pause_state.h"

#include <memory>

#include "../components/container.h"
#include "../components/image.h"
#include "../components/button.h"

PauseState::PauseState(StateStack &stack, Context context, std::string world) :
  State(stack, context),
  mWorldName(world),
  mGUIContainer(context, context.window->getDefaultView()) {
  sf::Vector2f windowSize(context.window->getSize());

  //Buttons
  auto resumeButton = std::make_shared<GUI::Button>(
    *getContext().textureManager,
    *getContext().fontManager);
  resumeButton->setCallback(
    [this]() {
    requestStackPop();
    getContext().musicPlayer->setPaused(false);
  });
  resumeButton->setText("Resume", sf::Color::White);
  mGUIContainer.add(resumeButton);

  auto levelsButton = std::make_shared<GUI::Button>(
    *getContext().textureManager,
    *getContext().fontManager);
  levelsButton->setCallback(
    [this]() {
    requestStackClear();
    requestStackPush(ID::LevelSelect, mWorldName);

    //Stop music to main theme will play
    getContext().musicPlayer->stop();
  });
  levelsButton->setText("Levels", sf::Color::White);
  mGUIContainer.add(levelsButton);

  auto worldsButton = std::make_shared<GUI::Button>(
    *getContext().textureManager,
    *getContext().fontManager);
  worldsButton->setCallback(
    [this]() {
    requestStackClear();
    requestStackPush(ID::WorldSelect, mWorldName);

    //Stop music to main theme will play
    getContext().musicPlayer->stop();
  });
  worldsButton->setText("Worlds", sf::Color::White);
  mGUIContainer.add(worldsButton);

  auto menuButton = std::make_shared<GUI::Button>(
    *getContext().textureManager,
    *getContext().fontManager);
  menuButton->setCallback(
    [this]() {
    requestStackClear();
    requestStackPush(ID::Menu);

    //Stop music to main theme will play
    getContext().musicPlayer->stop();
  });
  menuButton->setText("Main", sf::Color::White);
  mGUIContainer.add(menuButton);

  auto optionsButton = std::make_shared<GUI::Button>(
    *getContext().textureManager,
    *getContext().fontManager);
  optionsButton->setCallback(
    [this]() {
    requestStackPush(ID::Option);
  });
  optionsButton->setText("Options", sf::Color::White);
  //mGUIContainer.add(optionsButton);

  auto helpButton = std::make_shared<GUI::Button>(
    *getContext().textureManager,
    *getContext().fontManager);
  helpButton->setCallback(
    [this]() {
    requestStackPush(ID::Help);
  });
  helpButton->setText("Help", sf::Color::White);
  //mGUIContainer.add(helpButton);

  mGUIContainer.setPosition(windowSize * 0.5f);
  resumeButton->move(0.f, -120.f);
  levelsButton->move(0.f, -60.f);
  worldsButton->move(0.f, 0.f);
  menuButton->move(0.f, 60.f);
  //optionsButton->move(0.f, 60.f);
  //helpButton->move(0.f, 120.f);

  //Music
  context.musicPlayer->setPaused(true);
}

void PauseState::draw() {
  sf::RenderWindow &window = *getContext().window;
  window.setView(window.getDefaultView());
  mGUIContainer.setView(getContext().window->getDefaultView());
  window.draw(mGUIContainer);
}

bool PauseState::update(sf::Time deltaTime) {
  return false;
}

bool PauseState::handleEvent(const sf::Event &event) {
  /*if (event.type == sf::Event::KeyPressed){
      if ((event.key.code == sf::Keyboard::P) ||
          (event.key.code == sf::Keyboard::Escape))
          requestStackPop();
  }*/
  mGUIContainer.handleEvent(event);
  return false;
}

void PauseState::onResolutionChange() {
  sf::Vector2f windowSize(getContext().window->getSize());
  mGUIContainer.setPosition(windowSize * 0.5f);
}