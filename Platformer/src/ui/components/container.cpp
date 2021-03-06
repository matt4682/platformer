#include <SFML\Graphics.hpp>
#include "Container.h"
#include <vector>

namespace GUI {
  Container::Container(State::Context context, sf::View view) :
    mSoundPlayer(*context.soundPlayer),
    mChildren(),
    mSelectedChild(-1),
    mView(view) {}

  bool Container::isSelectable() {
    return false;
  }

  void Container::handleEvent(const sf::Event &event) {
    if (hasSelection() && mChildren[mSelectedChild]->isActive()) {
      mChildren[mSelectedChild]->handleEvent(event);
    }
    else if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W) {
        mSoundPlayer.play(SoundEffectID::Switch);
        selectPrevious();
      }
      else if (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S) {
        mSoundPlayer.play(SoundEffectID::Switch);
        selectNext();
      }
      else if (event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space) {
        if (hasSelection())
          mChildren[mSelectedChild]->activate();
      }
    }
  }

  void Container::add(Component::Ptr component) {
    mChildren.push_back(component);

    if (!hasSelection() && component->isSelectable())
      select(mChildren.size() - 1);
  }

  void Container::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.setView(mView);
    for (auto &component : mChildren) {
      target.draw(*component, states);
    }
  }

  bool Container::hasSelection() {
    return mSelectedChild >= 0;
  }

  void Container::select(std::size_t index) {
    if (mChildren[index]->isSelectable()) {
      if (hasSelection())
        mChildren[mSelectedChild]->deselect();
      mChildren[index]->select();
      mSelectedChild = index;
    }
  }

  void Container::selectNext() {
    if (!hasSelection())
      return;

    int next = mSelectedChild;
    do {
      next = (next + 1) % mChildren.size();
    } while (!mChildren[next]->isSelectable());

    float eleYPos = mChildren[next]->getPosition().x;
    float viewBottom = mView.getCenter().x + mView.getSize().x / 2.f;
    float viewTop = mView.getCenter().x - mView.getSize().x / 2.f;
    if (eleYPos + 50 > viewBottom) {
      float currentYPos = mChildren[mSelectedChild]->getPosition().x;
      mView.move(eleYPos - currentYPos, 0.f);
    }

    //Wrap around
    else if (eleYPos - 50 < viewTop) {
      float viewOffset = 0.f;
      for (int i = mSelectedChild; i >= next; --i) {
        if (mChildren[i]->getPosition().x - 50.f < viewTop) {
          viewOffset -= 404.f;
        }
      }
      mView.move(viewOffset, 0.f);
    }
    select(next);
  }

  void Container::selectPrevious() {
    if (!hasSelection())
      return;

    int prev = mSelectedChild;
    do {
      prev = (prev + mChildren.size() - 1) % mChildren.size();
    } while (!mChildren[prev]->isSelectable());

    float eleYPos = mChildren[prev]->getPosition().x;
    float viewTop = mView.getCenter().x - mView.getSize().x / 2.f;
    float viewBottom = mView.getCenter().x + mView.getSize().x / 2.f;
    if (eleYPos - 50 < viewTop) {
      float currentYPos = mChildren[mSelectedChild]->getPosition().x;
      mView.move(eleYPos - currentYPos, 0.f);
    }

    //Wrap around
    //Assumes all non-selectable elements are adjacent and at the end of container
    else if (eleYPos + 50 > viewBottom) {
      float viewOffset = 0.f;
      for (int i = mSelectedChild; i <= prev; ++i) {
        if (mChildren[i]->getPosition().x + 50 > viewBottom) {
          viewOffset += 404.f;
        }
      }
      mView.move(viewOffset, 0.f);
    }
    select(prev);
  }

  void Container::setView(sf::View view) {
    mView = view;
  }

  sf::View Container::getView() {
    return mView;
  }
}