#pragma once
#include "TileMap/TileMap.h"
#include "camera/camera.h"
#include "entity/entity.h"
#include "shaderProgram/shader.h"

class SandBox {
private:
  unsigned int w_width, w_height;

  bool startRender;
  bool m_updateEntitiesMove = false;
  bool m_showGridMap;
  bool m_enableTileMapEditing = false;
  bool m_showEntitiesQuads = false;
  bool m_spawn_slime = false;
  bool m_lastSpawnSlime = false;
  bool m_lastEnableTileEditing = false;
  bool m_gravityInfluence;
  float initialZoom;
  float time;
  glm::vec2 m_mousePosScreen;
  glm::vec2 m_mousePosGlobal;

  std::unique_ptr<TileMap> m_tileMap;

public:
  bool keys[1024];
  bool keyProcessed[1024];

  SandBox();
  SandBox(SandBox &sandBox);
  ~SandBox();

  void setCameraScroll(float yoffset);
  void GetMousePos(glm::vec2 pos);
  void updateMousePos();

  void processInput(float dt);
  void renderGUI();
  void initRenderData();
  void update(float &dt);
  void updateEntities(float &dt);
  void renderScene();
  void initSandBox(int width, int height);
  void handleEntitiesInstances();
  void m_resizeCamera(int width, int height);
  // void detectTileCol(Entity2D_Instaciaded& entity);
  // void detectEntityCol(Entity2D_Instaciaded& entity1, Entity2D_Instaciaded&
  // entity2, float& dt); void checkEntitiesTileLimits(Entity2D_Instaciaded&
  // entity);
};
