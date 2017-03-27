#ifndef LABEL_H__
#define LABEL_H__

#include <SFML/Graphics.hpp>

#include "../component.h"
#include "../../game/resource_manager.h"

namespace GUI {
  class Label : public Component {
  public:
    typedef std::shared_ptr<Label> Ptr;

  public:
    Label(const std::string &text, sf::Color color, unsigned int size, FontManager &fontManager);

    virtual bool isSelectable();
    void setText(const std::string &text);
    void setOrigin(float x, float y);
    sf::Vector2f getOrigin();
    virtual void handleEvent(const sf::Event &event);

  private:
    void draw(sf::RenderTarget &target, sf::RenderStates states) const;

  private:
    sf::Text mText;
  };
};

#endif  // LABEL_H__