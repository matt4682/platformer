#ifndef TILEMAP_NODE_H__
#define TILEMAP_NODE_H__

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>

#include "../scene_node.h"
#include "../../game/map_data.h"

class TilemapNode : public SceneNode {
public:
  TilemapNode(const MapData &mapData, std::vector<b2Body *> contours);

  unsigned int getCategory() const;
  virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

private:
  MapData mMapData;
  std::vector<b2Body *> mContours;
};

#endif  // TILEMAP_NODE_H__