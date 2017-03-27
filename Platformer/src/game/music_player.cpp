#include "./music_player.h"

#include <iostream>

#include "../log.h"

MusicPlayer::MusicPlayer() :
  mMusic(),
  mVolume(100.f) {
  mFileMap[MainTheme] = "res/Audio/Music/menu.ogg";
  mFileMap[LevelTheme] = "res/Audio/Music/level_theme.ogg";
}

void MusicPlayer::play(MusicID theme) {
  std::string fileName = mFileMap[theme];

  if (!mMusic.openFromFile(fileName)) {
    gamelog(ERROR) << "Error loading music: " << fileName << std::endl;
  }

  mMusic.setVolume(mVolume);
  mMusic.setLoop(true);
  mMusic.play();
}

void MusicPlayer::stop() {
  mMusic.stop();
}

void MusicPlayer::setPaused(bool paused) {
  paused ? mMusic.pause() : mMusic.play();
}

void MusicPlayer::setVolume(float volume) {
  mVolume = volume;
  mMusic.setVolume(mVolume);
}

float MusicPlayer::getVolume() {
  return mVolume;
}

bool MusicPlayer::isPlaying() {
  sf::Music::Status status = mMusic.getStatus();
  if (status == sf::Music::Status::Playing)
    return true;
  return false;
}