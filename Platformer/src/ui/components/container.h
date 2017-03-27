#ifndef CONTAINER_H__
#define CONTAINER_H__

#include <SFML\Graphics.hpp>
#include <vector>
#include <memory>

#include "../component.h"
#include "../state.h"

namespace GUI {
  class Container : public Component {
  public:
    typedef std::shared_ptr<Container> Ptr;

  public:
    Container(State::Context context, sf::View view);

    virtual bool isSelectable();
    virtual void handleEvent(const sf::Event &event);
    virtual void add(Component::Ptr component);
    void setView(sf::View view);
    sf::View getView();
    virtual void select(std::size_t index);

  protected:
    virtual bool hasSelection();
    virtual void selectNext();
    virtual void selectPrevious();

  protected:
    SoundPlayer &mSoundPlayer;
    std::vector<Component::Ptr> mChildren;
    int mSelectedChild;
    sf::View mView;

  private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
  };
}

#endif  // CONTAINER_H__