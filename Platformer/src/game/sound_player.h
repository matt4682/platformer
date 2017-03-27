#ifndef SOUND_PLAYER_H__
#define SOUND_PLAYER_H__

#include <SFML/Audio/Music.hpp>
#include <list>

#include "./resource_manager.h"

class SoundPlayer : private sf::NonCopyable {
public:
  SoundPlayer();

  void play(SoundEffectID effect);
  void removeStoppedSounds();
  void setVolume(float volume);
  float getVolume();

private:
  SoundBufferManager mSoundManager;
  std::list<sf::Sound> mSounds;
  float mVolume;
};

#endif  // SOUND_PLAYER_H__