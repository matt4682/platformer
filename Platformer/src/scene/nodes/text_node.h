#ifndef TEXT_NODE_H__
#define TEXT_NODE_H__

#include <SFML\Graphics.hpp>

#include "../scene_node.h"

class TextNode : public SceneNode {
public:
  explicit TextNode(const sf::Font &font);
  explicit TextNode(const sf::Font &font, const std::string &text);

  void setText(const std::string &text);
  void setColor(sf::Color color);

private:
  virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

private:
  sf::Text mText;
};

#endif  // TEXT_NODE_H__
