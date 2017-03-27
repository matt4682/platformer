#include "./text_node.h"

#include <SFML\Graphics.hpp>

TextNode::TextNode(const sf::Font &font) {
  mText.setFont(font);
}

TextNode::TextNode(const sf::Font &font, const std::string &text) {
  mText.setFont(font);
  mText.setString(text);
}

void TextNode::setText(const std::string &text) {
  mText.setString(text);
}

void TextNode::setColor(sf::Color color) {
  mText.setColor(color);
}

void TextNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(mText, states);
}