#include "./button.h"

#include <SFML\Graphics.hpp>

#include <functional>
#include <memory>
#include <iostream>

#include "../../game/resource_manager.h"

namespace GUI {
  Button::Button(TextureManager &textureManager) :
    mTextureNormalRect(sf::IntRect(0, 0, 208, 58)),
    mTexturePressedRect(sf::IntRect(0, 58, 208, 58)),
    mTextureDisabledRect(sf::IntRect(0, 0, 0, 0)),
    mIsToggle(false),
    mEnabled(true) {}

  Button::Button(TextureManager &textureManager, FontManager &fontManager) :
    mTextureNormalRect(sf::IntRect(0, 0, 208, 58)),
    mTexturePressedRect(sf::IntRect(0, 58, 208, 58)),
    mTextureDisabledRect(sf::IntRect(0, 0, 0, 0)),
    mText("", fontManager.get(FontID::Main), 24),
    mIsToggle(false),
    mEnabled(true) {
    mSprite.setTexture(textureManager.get(TextureID::GUISpriteSheet));
    mSprite.setTextureRect(mTextureNormalRect);
    mSprite.setScale(1.f, 1.f);
    sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(floor(bounds.width / 2), floor(bounds.height / 2));
  }

  void Button::setText(const std::string &text, sf::Color color) {
    mText.setString(text);
    mText.setColor(color);
    sf::FloatRect textBounds = mText.getLocalBounds();
    mText.setOrigin(textBounds.left + textBounds.width / 2,
      textBounds.top + textBounds.height / 2.f);
    mText.setPosition(-4.f, 1.f);
  }

  void Button::setCallback(Callback callback) {
    mCallback = callback;
  }

  void Button::setToggle(bool toggle) {
    mIsToggle = toggle;
  }

  bool Button::isSelectable() {
    return mEnabled;
  }

  void Button::enable() {
    mEnabled = true;
    mText.setColor(sf::Color::White);
    changeTexture(Type::Normal);
  }

  void Button::disable() {
    mEnabled = false;
    mText.setColor(sf::Color(77, 77, 77, 100));
    changeTexture(Type::Disabled);
  }

  void Button::select() {
    Component::select();
    changeTexture(Type::Pressed);
  }

  void Button::deselect() {
    Component::deselect();
    changeTexture(Type::Normal);
  }

  void Button::activate() {
    Component::activate();

    if (mIsToggle)
      changeTexture(Type::Pressed);

    if (mCallback)
      mCallback();

    if (!mIsToggle)
      deactivate();
  }

  void Button::deactivate() {
    Component::deactivate();

    if (mIsToggle) {
      if (isSelected())
        changeTexture(Type::Pressed);
      else
        changeTexture(Type::Normal);
    }
  }

  void Button::handleEvent(const sf::Event &event) {}

  void Button::draw(sf::RenderTarget &target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(mSprite, states);
    target.draw(mText, states);
  }

  void Button::changeTexture(Type buttonType) {
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