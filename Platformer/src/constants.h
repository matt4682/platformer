#ifndef CONSTANTS_H__
#define CONSTANTS_H__

#include <SFML\Graphics.hpp>

#include "./version.h"

// TODO(Matt): Use this when C++14 is available
#ifdef CPP14_CONSTEXPR
namespace helper {
  template <typename T>
  constexpr T sqrt_helper(const T &x, const T &lo, const T &hi) {
    if (lo == hi)
      return lo;

    const T mid = (lo + hi + 1) / 2;

    if (x / mid < mid)
      return sqrt_helper(x, lo, mid - 1);
    else
      return sqrt_helper(x, mid, hi);
  }

  template <typename T>
  constexpr T sqrt(const T &x) {
    return sqrt_helper<T>(x, 0, x / 2 + 1);
  }
}
#endif

namespace physics {
  constexpr float MINIMUM_JUMP_HEIGHT = 1.f;
  constexpr float MAXIMUM_JUMP_HEIGHT = 5.f;
  constexpr float JUMP_APEX_TIME = .5f;
  constexpr float ACCELERATION = .35f;
  constexpr float DECELERATION = .95f;
  constexpr float MAX_SPEED = 10.f;

  constexpr float GRAVITY = -(2.f * MAXIMUM_JUMP_HEIGHT) / (JUMP_APEX_TIME * JUMP_APEX_TIME);
#ifdef CPP14_CONSTEXPR
  constexpr float JUMP_VELOCITY = helper::sqrt(-(2.f * GRAVITY * MAXIMUM_JUMP_HEIGHT));
  constexpr float MAX_JUMP_VELOCITY = helper::sqrt((JUMP_VELOCITY * JUMP_VELOCITY) + 2.f * GRAVITY * (MAXIMUM_JUMP_HEIGHT - MINIMUM_JUMP_HEIGHT));
#else
  const float JUMP_VELOCITY = sqrt(fabs(2 * GRAVITY * MAXIMUM_JUMP_HEIGHT));
  const float MAX_JUMP_VELOCITY = sqrt(powf(JUMP_VELOCITY, 2) + 2 * GRAVITY * (MAXIMUM_JUMP_HEIGHT - MINIMUM_JUMP_HEIGHT));
#endif
}

namespace game {
  constexpr const char *GAME_NAME{ VER_INTERNALNAME_STR };
  constexpr const char *GAME_VERSION{ VER_FILEVERSION_STR };

  constexpr int FRAMERATE = 60;
  const sf::Time FRAME_RATE = sf::seconds(1.f / FRAMERATE);

  constexpr float PX_PER_M = 70.f;
  constexpr unsigned WIDTH = 1280;
  constexpr unsigned HEIGHT = 720;
}

#endif  // CONSTANTS_H__