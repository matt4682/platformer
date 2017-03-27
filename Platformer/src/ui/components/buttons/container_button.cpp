#include "./container_button.h"

#include <SFML\Graphics.hpp>

#include <functional>
#include <memory>
#include <iostream>

#include "../../../game/resource_manager.h"

namespace GUI {
  ContainerButton::ContainerButton(TextureManager &textureManager) : Button(textureManager), mChildren() {
    selectedTexture = textureManager.get(TextureID::SelectedWorld);
    mTextureNormalRect = sf::IntRect(0, 150, 725, 75);
    mTexturePressedRect = sf::IntRect(0, 150, 725, 75);
    mTextureDisabledRect = sf::IntRect(0, 225, 725, 75);
    mSprite.setTexture(textureManager.get(TextureID::GUISpriteSheet));
    mSprite.setTextureRect(mTextureNormalRect);

    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width / 2, bounds.height / 2);
    mPointer.setTexture(textureManager.get(TextureID::GUISpriteSheet));
    mPointer.setTextureRect(sf::IntRect(0, 120, 64, 26));
    sf::FloatRect pointerBounds = mPointer.getLocalBounds();
    mPointer.setOrigin(pointerBounds.width / 2, pointerBounds.height / 2);
    mPointer.move(-bounds.width / 2 - pointerBounds.width / 2.f - 10, 0.);
  }

  bool ContainerButton::isSelectable() {
    return true;
  }

  void ContainerButton::activate() {
    if (mEnabled) {
      Component::activate();

      if (mIsToggle)
        changeTexture(Type::Pressed);

      if (mCallback)
        mCallback();

      if (!mIsToggle)
        deactivate();
    }
  }

  void ContainerButton::deactivate() {
    if (mEnabled) {
      Component::deactivate();

      if (mIsToggle) {
        if (isSelected())
          changeTexture(Type::Pressed);
        else
          changeTexture(Type::Normal);
      }
    }
  }

  void ContainerButton::enable() {
    mEnabled = true;
    changeTexture(Type::Normal);
  }

  void ContainerButton::disable() {
    mEnabled = false;
    changeTexture(Type::Disabled);
  }

  void ContainerButton::select() {
    Component::select();
    if (mEnabled)
      changeTexture(Type::Pressed);
  }

  void ContainerButton::deselect() {
    Component::deselect();
    if (mEnabled)
      changeTexture(Type::Normal);
  }

  void ContainerButton::add(Component::Ptr component) {
    mChildren.push_back(component);
  }

  void ContainerButton::handleEvent(const sf::Event &event) {}

  void ContainerButton::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(mSprite, states);
    if (isSelected())
      target.draw(mPointer, states);
    for (auto &component : mChildren) {
      target.draw(*component, states);
    }
  }

  void ContainerButton::addImage(std::string name) {
    mTextureDisabledRect = sf::IntRect(0, 0, 394, 394);
    mTexturePressedRect = sf::IntRect(0, 0, 394, 394);
    mTextureNormalRect = sf::IntRect(0, 0, 394, 394);

    t.loadFromFile("res/Maps/" + name + ".png");

    mSprite.setTexture(t);

    mSprite.setTextureRect(mTextureNormalRect);
    mSprite.setScale(1.f, 1.f);

    mPointer.setTexture(selectedTexture);
    mPointer.setTextureRect(sf::IntRect(0, 0, 394, 394));
    sf::FloatRect pointerBounds = mPointer.getLocalBounds();
    mPointer.setOrigin(pointerBounds.width / 2, pointerBounds.height / 2);
    mPointer.move(394 / 2. + 42.5f, 394 / 2. - 38);
  }

  void ContainerButton::changeTexture(Type buttonType) {
    switch (buttonType) {
      case(Type::Normal) :
        mSprite.setTextureRect(mTextureNormalRect);
        break;
      case(Type::Pressed) :
        mSprite.setTextureRect(mTexturePressedRect);
        break;
      case(Type::Disabled) :
        mSprite.setTextureRect(mTextureDisabledRect);
        break;
    }
  }
}