#ifndef SOUND_NODE_H__
#define SOUND_NODE_H__

#include <SFML/Audio.hpp>

#include "../../game/sound_player.h"
#include "../scene_node.h"
#include "../../game/resource_manager.h"

class SoundNode : public SceneNode {
public:
  explicit SoundNode(SoundPlayer &player);

  void play(SoundEffectID sound);
  unsigned int getCategory() const;

private:
  SoundPlayer &mSoundPlayer;
};

#endif  // SOUND_NODE_H__