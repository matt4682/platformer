#include "./box2d_loader.h"

#include <SFML\Graphics.hpp>

#include <exception>
#include <algorithm>

#include "./tile_layer.h"
#include "./object_group.h"
#include "../../constants.h"

Box2DLoader::Box2DLoader() {
}

void Box2DLoader::load(const TileGrid &t_grid) {
  if (t_grid.size() == 0) {
    throw std::exception("Invalid size for TileGrid");
  }

  m_world_loaded = false;
  m_world = new b2World({ 0.0f, physics::GRAVITY });
  m_map_height = t_grid.size();
  m_map_width = t_grid[0].size();
  m_visited = BitGrid(m_map_height, BitVector(m_map_width, false));
  m_grid = t_grid;

  while (traceContours());

  m_world_loaded = true;
}

bool Box2DLoader::isWorldLoaded() const {
  return m_world_loaded;
}

gsl::owner<b2World *> Box2DLoader::getWorld() const {
  return m_world;
}

std::vector<gsl::owner<b2Body *>> Box2DLoader::getContours() const {
  return m_contours;
}

Point Box2DLoader::firstValidNeighbour(cint col, cint row) {
  const Points neighbours = {
    { Middle, Top },
    { Middle, Bottom },
    { Left, Center },
    { Right, Center }
  };

  for (const auto &point : neighbours) {
    auto neighbour = Point({col + point.first, row + point.second});

    auto valid_col = neighbour.first >= 0 && neighbour.first < m_map_width;
    auto valid_row = neighbour.second >= 0 && neighbour.second < m_map_height;
    auto valid_tile = m_grid[neighbour.second][neighbour.first].GID == 0;

    if (valid_col && valid_row && valid_tile) {
      return { neighbour.first, neighbour.second };
    }
  }

  return{ -1, -1 };
}

/* IMPLEMENTATION NOTES:
 * This method looks for a Tile in grid with a GID that we haven't visited yet.
 * The Tile must have a direct neighbours with a GID. of 0 (blank tile)
 * Trace the contour from that neighbour and return that we found one
 * If no suitable Tile is found, the method will return false
 * This should be called until it returns false ensure everything is traced
 */
bool Box2DLoader::traceContours() {
  for (auto row = 0; row < m_map_height; ++row) {
    for (auto col = 0; col < m_map_width; ++col) {
      if (!m_visited[row][col] && m_grid[row][col].GID != 0) {
        auto neighbour = firstValidNeighbour(col, row);
        auto first = neighbour.first;
        auto second = neighbour.second;

        if (neighbour != Point(-1, -1)) {
          traceContourComponent(col, row, first, second);
          return true;
        }
      }
    }
  }

  return false;
}

void Box2DLoader::applyOffset(cint t_GID) {
  const std::map<int, int> offset_map = {
    {16, 65},  // Bridge middle
    {17, 65},  // Bridge left
    {18, 65}   // Bridge right
  };

  auto iterator = offset_map.find(t_GID);

  if (iterator != offset_map.cend()) {
    m_tile_offset = iterator->second;
  }
  else {
    m_tile_offset = 0;
  }
}

/* IMPLEMENTATION NOTES:
 * This is a basic implementation of the Moore-Neighbor tracing algorithm
 * see: https://en.wikipedia.org/wiki/Moore_neighborhood
 */
void Box2DLoader::traceContourComponent(cint x, cint y, cint x2, cint y2) {
  int neighbour_x = x2;                             //
  int neighbour_y = y2;                             //
  m_visited[y][x] = true;
  std::vector<b2Vec2> vertices;

  auto neighborhood = getMooreNeighborhood(x, y);
  auto index = getMooreIndex(x, y, x2, y2);
  auto clockwise_index = (index + 6) % 8;

  applyOffset(m_grid[y][x].GID);

  {
    auto x_new = neighborhood[clockwise_index].first;
    auto y_new = neighborhood[clockwise_index].second;

    vertices.push_back(makeChainVertex(x, y, x_new, y_new));
  }

  {
    auto x_new = neighborhood[index].first;
    auto y_new = neighborhood[index].second;

    vertices.push_back(makeChainVertex(x, y, x_new, y_new));
  }

  int boundaryX = x;
  int boundaryY = y;
  int currentX = neighbour_x;
  int currentY = neighbour_y;
  int backtrackX = -1;
  int backtrackY = -1;
  int numStartVisits = 0;
  m_tile_offset = 0;
  /*
  while (
  (!
    (
      (boundaryX == x) && 
      (boundaryY == y) && 
      (backtrackX == neighbour_x) && 
      (backtrackY == neighbour_y)
    )
  ) && 
  (numStartVisits < 3)
) {
  
  */

  while ((!((boundaryX == x) && (boundaryY == y) && (backtrackX == neighbour_x) && (backtrackY == neighbour_y))) && (numStartVisits < 3)) {
    if (boundaryX == x && boundaryY == y) {
      if (numStartVisits > 0) {
        neighbour_x = backtrackX;
        neighbour_y = backtrackY;
      }
      ++numStartVisits;
    }

    auto neighborhoodCoords = getMooreNeighborhood(boundaryX, boundaryY);
    int i = getMooreIndex(boundaryX, boundaryY, currentX, currentY);
    int numVisited = 0;
    auto invalidPair = std::make_pair(-1, -1);

    while (!m_grid[currentY][currentX].GID) {
      backtrackX = currentX;
      backtrackY = currentY;
      currentX = neighborhoodCoords[i].first;
      currentY = neighborhoodCoords[i].second;
      ++numVisited;

      if (neighborhoodCoords[i] == invalidPair) {
        createStaticBody(vertices);
        return;
      }

      i = (i == 7 ? 0 : i + 1);

      if (numVisited == 9) {
        createStaticBody(vertices);
        return;
      }

      //applyOffset(grid[currentY][currentX].GID);
      if (m_grid[currentY][currentX].GID) {
        auto gid = m_grid[currentY][currentX].GID;
        if (gid == 16 || gid == 17 || gid == 18) { // Bridge & connectors
          m_tile_offset = 65;
        }
        else {
          m_tile_offset = 0;
        }
      }

      if (((!m_grid[currentY][currentX].GID) && (i % 2 == 1) && (numVisited > 2))) {
        b2Vec2 edgeV1, edgeV2;
        vertices.push_back(makeChainVertex(boundaryX, boundaryY, neighborhoodCoords[i - 1].first, neighborhoodCoords[i - 1].second));
      }
    }

    m_visited[currentY][currentX] = true;
    boundaryX = currentX;
    boundaryY = currentY;
    currentX = backtrackX;
    currentY = backtrackY;

    vertices.push_back(makeChainVertex(boundaryX, boundaryY, backtrackX, backtrackY));
  }
  createStaticBody(vertices);
}

b2Vec2 Box2DLoader::makeChainVertex(cint x, cint y, cint x2, cint y2) {
  b2Vec2 v1;

  auto relative_location = std::make_pair(x2 - x, y2 - y);
  auto offset_percentage = m_tile_offset / 100.f;
  auto y_offset = m_map_height - y;
  auto pair_sum = relative_location.first + relative_location.second;
  auto pair_diff = relative_location.first - relative_location.second;

  float vertex_y = pair_sum == -1 ? y_offset - offset_percentage : y_offset - 1;
  float vertex_x = static_cast<float>(pair_diff == -1 ? x : x + 1);

  v1.Set(vertex_x, vertex_y);

  return v1;
}

void Box2DLoader::createStaticBody(std::vector<b2Vec2> &chainVertices) {
  b2BodyDef bodyDef;
  bodyDef.type = b2BodyType::b2_staticBody;
  bodyDef.allowSleep = true;
  bodyDef.awake = true;
  b2Body *platform = m_world->CreateBody(&bodyDef);
  /*
  auto chainLoop = std::find(chainVertices.begin() + 1, chainVertices.end(), chainVertices[0]);
  if (chainLoop != chainVertices.end())
    chainVertices.erase(chainLoop + 1, chainVertices.end());*/
  int numVertices = chainVertices.size();

  b2Vec2 vertices[8];
  vertices[0] = chainVertices[0];

  if (numVertices >= 8) {
    for (int i = 0; i < numVertices / 7; ++i) {
      if (i * 8 + (7 - i) >= numVertices)
        continue;
      for (int j = 1; j < 8; ++j) {
        vertices[j] = chainVertices[i * 8 + (j - i)];
      }
      b2ChainShape chain;
      chain.CreateChain(vertices, 8);
      b2Fixture *contourFixture = platform->CreateFixture(&chain, 0);
      contourFixture->SetFriction(1.f);

      vertices[0] = vertices[7];
    }
  }

  int lastChainLength = (numVertices < 8) ? numVertices : numVertices % 7;

  if (numVertices < 8) {
    for (int i = 0; i < numVertices; ++i)
      vertices[i] = chainVertices[i];
  }
  else {
    for (int i = 1; i < lastChainLength; ++i) {
      int index = numVertices - lastChainLength + i;
      vertices[i] = chainVertices[index];
    }
  }
  if (lastChainLength >= 2) {
    b2ChainShape lastChain;
    lastChain.CreateChain(vertices, lastChainLength);
    b2Fixture *lastContourFixture = platform->CreateFixture(&lastChain, 0);
  }
  m_contours.push_back(platform);
}

Points Box2DLoader::getMooreNeighborhood(cint startX, cint startY) {
  Points neighborhood;

  for (const auto &pair : m_relation_index) {
    auto x = startX + pair.first;
    auto y = startY + pair.second;

    if (x >= 0 && x < m_map_width && y >= 0 && y < m_map_height) {
      neighborhood.push_back({ x, y });
    } else {
      neighborhood.push_back({ -1, -1 });
    }

  }
    
  return neighborhood;
}

int Box2DLoader::getMooreIndex(cint x, cint y, cint x2, cint y2) {
  auto delta_x = x2 - x;
  auto delta_y = y2 - y;

  auto begin = std::begin(m_relation_index);
  auto end = std::end(m_relation_index);
  auto value = std::make_pair(delta_x, delta_y);

  auto iterator = std::find(begin, end, value);

  return iterator - begin;
}