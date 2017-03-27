#ifndef ANIMATION_H__
#define ANIMATION_H__

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <vector>

class Animation {
public:
  Animation();

  void addFrame(sf::IntRect rect);
  void setSpriteSheet(const sf::Texture &texture);
  const sf::Texture *getSpriteSheet() const;
  std::size_t getSize() const;
  const sf::IntRect &getFrame(std::size_t n) const;

private:
  std::vector<sf::IntRect> m_frames;
  const sf::Texture *m_texture;
};

#endif  // ANIMATION_H__
