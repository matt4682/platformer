#ifndef NUMBER_H__
#define NUMBER_H__

#include <SFML\Graphics.hpp>

#include "../../game/resource_manager.h"
#include "../component.h"

namespace GUI {
  class Number : public Component {
  public:
    typedef std::shared_ptr<Number> Ptr;

  public:
    Number(TextureManager &textureManager, unsigned int num);
    Number(TextureManager &textureManager, float num);

    virtual void handleEvent(const sf::Event &event);
    virtual bool isSelectable();

  private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    sf::IntRect getRectFromNum(unsigned int num);

  private:
    sf::Sprite mOnesSprite;
    sf::Sprite mTensSprite;
    sf::Sprite mTenthsSprite;
    sf::Sprite mHundredthsSprite;
    sf::Sprite mDecimalSprite;
  };
}

#endif  // NUMBER_H__