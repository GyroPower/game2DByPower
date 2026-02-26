#pragma once
#include "Spriterenderer/spriteRenderer.h"
#include "entity/entity.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>

class TileMap {
private:
  std::vector<std::vector<std::unique_ptr<Tile>>> m_levelTileMap;
  std::vector<std::unique_ptr<Entity2D_Instaciaded>> m_tilesToRender;

  glm::vec2 m_mapSize;
  int m_tileSize;
  int m_tileRenderIndex;

public:
  TileMap(glm::vec2 size = glm::vec2(24.0f, 16.0f), int tileSize = 16.0f);
  ~TileMap();

  Tile *m_GetTile(glm::vec3 worldPos);
  Tile *m_GetGridTile(glm::vec2 gridPos);
  glm::vec2 m_getGridPos(glm::vec2 worldPos);
  int m_getTileSize();

  float getWidth();
  float getHeight();

  void m_addTileToRender(glm::vec3 worldPos);
  void m_addTile(glm::vec3 worldPos);
  void m_removeTile(glm::vec3 worldPos);
  void m_updateTilesBufferRenderer(SpriteRendererInstanced &tileRenderer);
  void m_deleteTileFromRender(glm::vec3 worldPos);
  void m_updateTileRenderBuffer(SpriteRendererInstanced &tileRenderer);
  void m_updateTileMap(glm::vec2 size, int tileSize);
};
