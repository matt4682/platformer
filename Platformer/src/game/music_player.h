#ifndef MUSIC_PLAYER_H__
#define MUSIC_PLAYER_H__

#include <SFML/Audio/Music.hpp>

#include <string>
#include <map>

enum MusicID {
  MainTheme,
  LevelTheme
};

class MusicPlayer : private sf::NonCopyable {
public:
  MusicPlayer();

  void play(MusicID theme);
  void stop();
  void setPaused(bool paused);
  void setVolume(float volume);
  float getVolume();
  bool isPlaying();

private:
  sf::Music mMusic;
  std::map<MusicID, std::string> mFileMap;
  float mVolume;
};

#endif  // MUSIC_PLAYER_H__