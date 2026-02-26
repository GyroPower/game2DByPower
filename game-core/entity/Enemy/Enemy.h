#pragma once
#include "entity/entity.h"

class EnemySlime2D : public Entity2D_Instaciaded {

public:
  int m_life;

  // Timers
  float m_damageTime;
  float m_damageTimer;

  glm::vec3 m_maxLeftPos;
  glm::vec3 m_maxRightPos;
  glm::vec2 m_texturePosOrigin;

  EnemySlime2D(int *renderIndex, float texSlot = -1.0f,
               glm::vec3 position = glm::vec3(0.0f),
               glm::vec3 maxLeftPos = glm::vec3(10.0f, 0.0f, 0.0f),
               glm::vec3 maxRightPos = glm::vec3(10.0f, 0.0f, 0.0f),
               glm::vec2 texturePosition = glm::vec2(0.0f, 96.0f),
               glm::vec2 textureSIze = glm::vec2(28.0f, 16.0f),
               glm::vec2 size = glm::vec2(16.0f, 16.0f),
               glm::vec2 posOffset = glm::vec2(3.5f, 3.0f),
               glm::vec2 sizeOffset = glm::vec2(-5.5f, -11.0f),
               glm::vec4 color = glm::vec4(0.3f, 0.0f, 1.0f, 1.0f));

  void move(float &dt);
  void m_anim(float &dt);
  void m_onCollision();
  Rect m_getEntityRect();
  void m_updateTimer(float &dt);
};
