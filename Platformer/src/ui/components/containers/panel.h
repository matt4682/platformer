#ifndef PANEL_H__
#define PANEL_H__

#include <SFML\Graphics.hpp>

#include "../../../game/resource_manager.h"
#include "../container.h"
#include "../../state.h"

namespace GUI {
  class Panel : public Container {
  public:
    typedef std::shared_ptr<Panel> Ptr;

  public:
    Panel(State::Context context, sf::View view);

  private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

  private:
    sf::Texture &mTexture;
    sf::Sprite mSprite;
  };
}

#endif  // PANEL_H__n