#ifndef OPTION_H__
#define OPTION_H__

#include <SFML\Graphics.hpp>
#include <functional>
#include <memory>
#include <map>

#include "../component.h"
#include "../../game/resource_manager.h"

namespace GUI {
  class Option : public Component {
  public:
    typedef std::shared_ptr<Option> Ptr;

  public:
    Option(std::string name, FontManager &fontManager);

    virtual bool isSelectable();
    virtual void select();
    virtual void deselect();
    virtual bool hasSelection();
    virtual void selectNext() = 0;
    virtual void selectPrevious() = 0;
    virtual void select(std::size_t index) = 0;
    virtual void activate() = 0;
    virtual void deactivate() = 0;
    virtual void handleEvent(const sf::Event &event);

  protected:
    int mSelectedChild;
    sf::Text mNameText;
    sf::Text mValueText;
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
  };
}

#endif  // OPTION_H__