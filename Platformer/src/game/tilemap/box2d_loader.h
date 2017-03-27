#ifndef BOX2D_LOADER_H__
#define BOX2D_LOADER_H__

#include <Box2D\Box2D.h>
#include <gsl.h>

#include <vector>
#include <utility>

#include "./tile_layer.h"
#include "./object_group.h"

using cint = const int;
using BitVector = std::vector<bool>;
using BitGrid = std::vector<BitVector>;
using Point = std::pair<int, int>;
using Points = std::vector<Point>;

class Box2DLoader {
public:
  Box2DLoader();

  void load(const TileGrid &grid);
  bool isWorldLoaded() const;
  gsl::owner<b2World *> getWorld() const;
  std::vector<gsl::owner<b2Body *>> getContours() const;

private:
  bool traceContours();
  void traceContourComponent(cint x, cint y, cint x2, cint y2);
  b2Vec2 makeChainVertex(cint x, cint y, cint x2, cint y2);
  void createStaticBody(/* const */ std::vector<b2Vec2> &chainVertices);
  Points getMooreNeighborhood(cint x, cint y);
  int getMooreIndex(cint x, cint y, cint x2, cint y2);
  void applyOffset(cint GID);
  Point firstValidNeighbour(cint col, cint row);

private:
  b2World *m_world{ nullptr };
  std::vector<b2Body *> m_contours{};
  bool m_world_loaded{ false };
  int m_map_width{ 0 };
  int m_map_height{ 0 };
  int m_tile_offset{ 0 };
  BitGrid m_visited{};
  TileGrid m_grid{};

  enum HorizontalDirections {
    Left = -1,
    Middle,
    Right
  };

  enum VeritalDirections {
    Top = -1,
    Center,
    Bottom
  };

  const Points m_relation_index = {
    { Left, Center },
    { Left, Top },
    { Middle, Top },
    { Right, Top },
    { Right, Center },
    { Right, Bottom },
    { Middle, Bottom },
    { Left, Bottom }
  };
};

#endif  // BOX2D_LOADER_H__