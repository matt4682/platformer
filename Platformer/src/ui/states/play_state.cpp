#include "./play_state.h"

#include "../../game/level_manager.h"
#include "../../save_manager.h"
#include "../../game/player.h"

PlayState::PlayState(StateStack &stack, Context context, const std::string &map) :
  State(stack, context),
  mPlayer(*context.player),
  mMapName(map) {
  mWorld.reset(context.levelManager->get(mMapName));
  context.musicPlayer->play(MusicID::LevelTheme);
}

void PlayState::draw() {
  mWorld->draw();
}

bool PlayState::update(sf::Time deltaTime) {
  if (!mWorld->isComplete())
    mWorld->update(deltaTime);
  else {
    requestStackPush(ID::LevelCompletion);
    SaveManager::sharedInstance()->finishLevel(mMapName, mWorld->getNumDeaths(), mWorld->getAttemptTime(), mWorld->isGemCollected());
  }
  CommandQueue &commands = mWorld->getCommandQueue();
  mPlayer.handleInput(commands);

  return false;
}

bool PlayState::handleEvent(const sf::Event &event) {
  CommandQueue &commands = mWorld->getCommandQueue();
  mPlayer.handleEvent(event, commands);

  if ((event.type == sf::Event::KeyPressed) &&
    ((event.key.code == sf::Keyboard::P) || (event.key.code == sf::Keyboard::Escape))) {
    requestStackPush(ID::Pause, mMapName.substr(0, mMapName.find("/")));
  }
  else if (event.type == sf::Event::LostFocus) {
    requestStackPush(ID::Pause, mMapName.substr(0, mMapName.find("/")));
  }

  return false;
}

void PlayState::onResolutionChange() {
  mWorld->onResolutionChange();
}