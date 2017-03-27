#ifndef CONTAINER_BUTTON_H__
#define CONTAINER_BUTTON_H__

#include <SFML\Graphics.hpp>
#include <functional>
#include <memory>

#include "../button.h"
#include "../../../game/resource_manager.h"

namespace GUI {
  class ContainerButton : public Button {
  public:
    typedef std::shared_ptr<ContainerButton> Ptr;

  public:
    ContainerButton(TextureManager &textureManager);

    virtual bool isSelectable();
    virtual void enable();
    virtual void disable();
    virtual void select();
    virtual void deselect();
    virtual void activate();
    virtual void deactivate();
    virtual void add(Component::Ptr component);
    virtual void handleEvent(const sf::Event &event);
    virtual void addImage(std::string name);

  private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
    virtual void changeTexture(Type buttonType);

  private:
    sf::Texture t;
    sf::Texture selectedTexture;
    sf::Sprite mPointer;
    std::vector<Component::Ptr> mChildren;
  };
}

#endif  // CONTAINER_BUTTON_H__