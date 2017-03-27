#ifndef VECTOR_OPTION_H__
#define VECTOR_OPTION_H__

#include <SFML\Graphics.hpp>
#include <functional>
#include <memory>
#include <vector>

#include "../../../game/resource_manager.h"
#include "../option.h"

namespace GUI {
  class VectorOption : public Option {
  public:
    typedef std::shared_ptr<VectorOption> Ptr;
    typedef std::function<void(sf::Vector2u)> Callback;

  public:
    VectorOption(std::string name, FontManager &fontManager);

    void setCallback(Callback callback);
    virtual void selectNext();
    virtual void selectPrevious();
    virtual void select(std::size_t index);
    virtual void activate();
    virtual void deactivate();
    void addPair(std::pair<std::string, sf::Vector2u> pair);

  private:
    Callback mCallback;
    std::vector<std::pair<std::string, sf::Vector2u>> mOptionPairs;
  };
}

#endif  // VECTOR_OPTION_H__