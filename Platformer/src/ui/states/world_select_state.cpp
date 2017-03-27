#include "./world_select_state.h"

#include <memory>
#include <iostream>
#include <tuple>

#include "../components/container.h"
#include "../components/image.h"
#include "../components/button.h"
#include "../components/label.h"
#include "../components/number.h"
#include "../components/buttons/container_button.h"
#include "../../save_manager.h"

using namespace std;

WorldSelectState::WorldSelectState(StateStack &stack, Context context, std::string world) :
  State(stack, context),
  mGUIContainer(context, context.window->getDefaultView()),
  mWorldContainer(std::make_shared<GUI::Container>(context, context.window->getDefaultView())),
  numWorlds(0),
  world(world),
  defaultSelection(0) {
  sf::Vector2f windowSize = context.window->getDefaultView().getSize();

  //Background
  sf::Texture &bgTexture = getContext().textureManager->get(TextureID::Background);
  sf::IntRect bgRect(0, 0, static_cast<int>(windowSize.x), static_cast<int>(windowSize.y));
  bgTexture.setRepeated(true);
  mBackground = sf::Sprite(bgTexture, bgRect);

  //Panel to display worlds
  buildWorldPanel();

  mWorldContainer->select(defaultSelection);

  //Play main theme if we didn't get here from the menu state
  if (!context.musicPlayer->isPlaying())
    context.musicPlayer->play(MusicID::MainTheme);
}

void WorldSelectState::draw() {
  sf::RenderWindow &window = *getContext().window;
  window.setView(window.getDefaultView());
  window.draw(mBackground);
  window.draw(mBackdrop);
  window.draw(mWorldPanel);
  window.draw(mGUIContainer);
}

bool WorldSelectState::update(sf::Time deltaTime) {
  return false;
}

bool WorldSelectState::handleEvent(const sf::Event &event) {
  if (event.type == sf::Event::KeyPressed) {
    mWorldContainer->handleEvent(event);
    if (event.key.code == sf::Keyboard::Escape) {
      requestStackPop();
      requestStackPush(ID::Menu);
    }
  }
  return false;
}

void WorldSelectState::addWorld(const std::string &name, bool enabled) {
  if (world == name) defaultSelection = numWorlds;

  sf::Vector2f windowSize(getContext().window->getDefaultView().getSize());
  auto worldButton = std::make_shared<GUI::ContainerButton>(*getContext().textureManager);

  worldButton->setCallback([this, name]() {
    requestStackPop();
    requestStackPush(State::ID::LoadingWorld, name);
  });

  worldButton->addImage(name);

  if (enabled) {
  }
  else {
    worldButton->disable();
  }

  sf::Texture &HUDTexture = getContext().textureManager->get(TextureID::HUDSpriteSheet);
  sf::IntRect rect;

  mWorldContainer->add(worldButton);
  /*float posX = windowSize.x * 0.5f;
  float firstButtonPadding = windowSize.y / 2.f - mWorldPanel.getGlobalBounds().height / 2.f + 140.f;
  float posY = static_cast<float>(firstButtonPadding + 138 * numWorlds);*/

  float posY = (mWorldPanel.getGlobalBounds().height * 0.5f) - 128;
  float posX = static_cast<float>((394 + 10) * numWorlds) + 604.f;

  worldButton->setPosition(posX, posY);

  ++numWorlds;
}

void WorldSelectState::buildWorldPanel() {
  sf::Vector2f windowSize(getContext().window->getDefaultView().getSize());
  sf::Texture &bgPanelTexture = getContext().textureManager->get(TextureID::LevelSelectionPanel);
  mWorldPanel = sf::Sprite(bgPanelTexture);
  mWorldPanel.setOrigin(sf::Vector2f(
    mWorldPanel.getGlobalBounds().width / 2,
    mWorldPanel.getGlobalBounds().height / 2));
  mWorldPanel.setPosition(windowSize * 0.5f);
  mWorldPanel.setColor(sf::Color(255, 255, 255, 255));

  //Set a centered view for our world display panel to handle
  //overflowing worlds and scrolling down
  sf::View worldPanelView;
  worldPanelView.setCenter(mWorldPanel.getPosition());
  worldPanelView.setSize(sf::Vector2f(static_cast<float>(bgPanelTexture.getSize().x), static_cast<float>(bgPanelTexture.getSize().y)));

  //Set viewport ratio according to the ratio of panel size
  //to window size, with a small amount of padding
  worldPanelView.setViewport(sf::FloatRect(
    (windowSize.x - bgPanelTexture.getSize().x) / 2.f / windowSize.x + 0.003f,    //NOTE: Change values for padding on worlds panel
    ((windowSize.y - bgPanelTexture.getSize().y) / 2.f / windowSize.y) + 0.01f,
    bgPanelTexture.getSize().x / windowSize.x - 0.006f,
    bgPanelTexture.getSize().y / windowSize.y - 0.02f));

  mWorldContainer->setView(worldPanelView);
  int collectedGems = getCollectedGems();

  //World selection buttons
  auto worlds = SaveManager::sharedInstance()->worldMap();

  for (const auto &world : worlds) {
    addWorld(world.first, collectedGems >= world.second);
  }

  mGUIContainer.add(mWorldContainer);
}

int WorldSelectState::getCollectedGems() {
  int collectedGems = 0;
  for (const auto &data : SaveManager::sharedInstance()->levelMap()) {
    auto data_pair = data.second;
    auto level_data = data_pair.first;
    if (get<LevelDataIndex::HasGem>(level_data))
      ++collectedGems;
  }

  return collectedGems;
}

void WorldSelectState::onResolutionChange() {
}