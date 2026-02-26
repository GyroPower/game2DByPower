#include "Enemy.h"
#include "debugLog/debugLog.h"
#include "mathFuncs/mathFuncs.h"

/*
 maxLeftPos and maxRightPos in the constructor are the max offset in the left
 and right of how the enemy can move depending the initial pos, if are not
 defined, with the first thing that collide will change the direction in the x
 axis
*/

EnemySlime2D::EnemySlime2D(int *renderIndex, float texSlot, glm::vec3 position,
                           glm::vec3 maxLeftPos, glm::vec3 maxRightPos,
                           glm::vec2 texturePosition, glm::vec2 textureSize,
                           glm::vec2 size, glm::vec2 posOffset,
                           glm::vec2 sizeOffset, glm::vec4 color)
    : Entity2D_Instaciaded(renderIndex, position, size, color, posOffset,
                           sizeOffset, texSlot, texturePosition, textureSize),
      m_life(0), m_maxLeftPos(maxLeftPos), m_maxRightPos(maxRightPos),
      m_damageTime(1.3f), m_damageTimer(0.0f) {
  // defining the max left and right pos
  this->m_maxLeftPos = this->m_position - this->m_maxLeftPos;
  this->m_maxRightPos = this->m_position + this->m_maxRightPos;
  this->m_direction.x = 1;
  this->m_speed = glm::vec3(10.0f, 0.0f, 0.0f);
  this->m_animTimeLimit = 1.0f;
  this->m_animTime = 0.0f;
  this->m_tagName = "slime";
  this->m_moveRoutine = true;
  this->m_gravityInfluence = true;
  this->m_grounded = false;
  this->m_texturePosOrigin = this->m_texCoords.position;
}

Rect EnemySlime2D::m_getEntityRect() {
  if (this->m_direction.x == 1)
    return Rect(glm::vec2(this->m_position.x, this->m_position.y),
                glm::vec2(this->m_size.x, this->m_size.y),
                glm::vec2(this->m_previusPos.x, this->m_previusPos.y),
                this->m_posOffsetRect, this->m_sizeOffsetRect);

  float offset = (this->m_posOffsetRect.x + this->m_sizeOffsetRect.x) * -1;

  return Rect(glm::vec2(this->m_position.x, this->m_position.y),
              glm::vec2(this->m_size.x, this->m_size.y),
              glm::vec2(this->m_previusPos.x, this->m_previusPos.y),
              glm::vec2(offset, this->m_posOffsetRect.y),
              this->m_sizeOffsetRect);
}

void EnemySlime2D::move(float &dt) {
  this->m_previusPos = this->m_position;

  if (this->m_position.x > this->m_maxRightPos.x &&
      this->m_maxRightPos.x > this->m_maxLeftPos.x)
    this->m_direction.x = -1;
  else if (this->m_position.x < this->m_maxLeftPos.x &&
           this->m_maxLeftPos.x < this->m_maxRightPos.x)
    this->m_direction.x = 1;

  if (!this->m_toDelete)
    this->m_position += this->m_speed * this->m_direction;

  this->state = MOVE;

  this->setPosInterpolation(dt);
}

void EnemySlime2D::m_anim(float &dt) {
  if (this->state == MOVE && !this->m_toDelete) {
    if (this->m_animTime >= this->m_animTimeLimit) {

      if (this->m_texCoords.position.x >= 32.0f * 7) {
        this->m_texCoords.position.x = 0.0f;
      } else
        this->setNewTexOffset(glm::vec2(32.0, 0.0f));
      this->m_animTime = 0.0f;
    } else {
      this->m_animTime += dt * 6;
    }
  } else if (this->state == MOVE && this->m_toDelete) {
    if (this->m_texCoords.position.y != m_texturePosOrigin.y + 32.0f * 2) {
      this->m_texCoords.position.x = 0.0f;
      this->m_texCoords.position.y = m_texturePosOrigin.y + 32.0f * 2;
    }

    if (this->m_animTime >= this->m_animTimeLimit) {
      if (this->m_texCoords.position.x >= 32.0f * 4) {
        this->m_delete = true;

      } else
        this->setNewTexOffset(glm::vec2(32.0f, 0.0f));
      this->m_animTime = 0.0f;
    } else {
      this->m_animTime += dt * 6;
    }
  }
}

void EnemySlime2D::m_onCollision() {

  if (this->m_col && this->m_top) {
    this->m_top = false;
    LOG(this->m_tagName + std::to_string(this->m_renderInstanceIndex) +
        " was hurt");
    this->m_toDelete = true;
  }

  this->m_col = false;
}

void EnemySlime2D::m_updateTimer(float &dt) {
  if (this->m_damageTimer >= this->m_damageTime) {
    this->m_updateTimers = false;
    this->m_damageTimer = 0.0f;
    LOG("Top damage done");
  } else if (this->m_damageTimer < this->m_damageTime && this->m_updateTimers)
    this->m_damageTimer += dt;
}
