#include "player.h"
#include "debugLog/debugLog.h"
#include "mathFuncs/mathFuncs.h"
#include <GLFW/glfw3.h>

Player::Player(int *renderIndex, glm::vec3 position, glm::vec2 size,
               glm::vec4 color, glm::vec2 posOffsetRect,
               glm::vec2 sizeOffsetRect, float texSlot, glm::vec2 texPos,
               glm::vec2 texSize)
    : Entity2D_Instaciaded(renderIndex, position, size, color, posOffsetRect,
                           sizeOffsetRect, texSlot, texPos, texSize),
      m_showPlayerHitboxQuad(false), m_hitTime(0.0f), m_hitTimeDur(3.0f) {
  this->m_tagName = "player";
  this->m_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
  this->m_gravityInfluence = true;
}

Rect Player::m_getEntityRect() {

  if (state == IDLE || state == FALL || state == JUMP || state == WALL_TOUCH) {
    if (this->m_direction.x == 1.0f || this->m_direction.x == 0.0f)
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

  } else if (state == MOVE) {
    if (this->m_direction.x == 1.0f || this->m_direction.x == 0.0f)
      return Rect(
          glm::vec2(this->m_position.x, this->m_position.y),
          glm::vec2(this->m_size.x, this->m_size.y),
          glm::vec2(this->m_previusPos.x, this->m_previusPos.y),
          glm::vec2(this->m_posOffsetRect.x, this->m_posOffsetRect.y + 2),
          glm::vec2(this->m_sizeOffsetRect.x, this->m_sizeOffsetRect.y - 2));

    float offset = (this->m_posOffsetRect.x + this->m_sizeOffsetRect.x) * -1;

    return Rect(
        glm::vec2(this->m_position.x, this->m_position.y),
        glm::vec2(this->m_size.x, this->m_size.y),
        glm::vec2(this->m_previusPos.x, this->m_previusPos.y),
        glm::vec2(offset, this->m_posOffsetRect.y + 2),
        glm::vec2(-this->m_posOffsetRect.x, this->m_sizeOffsetRect.y - 2));
  }
}

void Player::m_processInput(bool keys[1024], bool keysProcessed[1024],
                            float &dt) {
  // player control
  static float runAcceleration = 240.0f;
  constexpr float runReduce = 450.0f;
  constexpr float gravity = 9.8f;
  if (keys[GLFW_KEY_D]) {

    float mult = 1.0f;
    if (this->m_speed.x < 0.0f && this->m_grounded ||
        this->m_speed.x < 0.0f && !m_gravityInfluence)
      mult = 10.0f;
    else if (this->m_wallTouch)
      mult = 3.0f;
    else
      mult = 2.0f;

    float actualRunAcceleration =
        !this->m_grounded && !this->m_wallTouch && m_gravityInfluence
            ? runAcceleration / 1.5f
            : runAcceleration;

    float actualSpeed =
        !this->m_grounded && !this->m_wallTouch && m_gravityInfluence
            ? this->m_maxSpeed / 2
            : this->m_maxSpeed / 2 + this->m_maxSpeed / 4;

    actualSpeed = !this->m_grounded && this->m_wallTouch ? this->m_maxSpeed / 2
                                                         : actualSpeed;

    this->m_pushed = true;
    this->m_speed.x = byPowerMath::approach(this->m_speed.x, actualSpeed,
                                            actualRunAcceleration * mult * dt);

  } else if (keys[GLFW_KEY_A]) {
    // this for the movement in the negative side of the x axis, and if mult is
    // to change much faster the vel to the positive to the negative side

    float mult = 1.0f;

    // if the player is grounded or the gravity is not affecting and the speed
    // was in the positive side will change drastically to the negative
    if ((this->m_speed.x > 0.0f && this->m_grounded) ||
        (this->m_speed.x > 0.0f && !m_gravityInfluence))
      mult = 10.0f;
    // if the player is touching a wall it will change but not that quickly like
    // the firts if statement
    else if (this->m_wallTouch)
      mult = 1.0f;
    // if is on the air it will be much lower
    else
      mult = 2.0f;

    float actualRunAcceleration =
        !this->m_grounded && !this->m_wallTouch && m_gravityInfluence
            ? runAcceleration / 1.5f
            : runAcceleration;

    float actualSpeed =
        !this->m_grounded && !this->m_wallTouch && m_gravityInfluence
            ? this->m_maxSpeed / 2
            : this->m_maxSpeed / 2 + this->m_maxSpeed / 4;

    actualSpeed = !this->m_grounded && this->m_wallTouch ? this->m_maxSpeed / 2
                                                         : actualSpeed;

    this->m_pushed = true;
    this->m_speed.x = byPowerMath::approach(this->m_speed.x, -1 * actualSpeed,
                                            actualRunAcceleration * mult * dt);
  }
  if (!keys[GLFW_KEY_D] && !keys[GLFW_KEY_A]) {
    // reduce the vel on the x axis y we don't push the player in the positive
    // or negative side of the axis
    this->m_pushed = false;
    this->m_speed.x = byPowerMath::approach(this->m_speed.x, 0, runReduce * dt);
  }
  if (keys[GLFW_KEY_W] && !m_gravityInfluence) {

    // moving negative in the y axis if there is not gravity influence

    float mult = 1.0f;
    if (this->m_speed.y > 0.0f)

      mult = 10.0f;

    this->m_speed.y = byPowerMath::approach(this->m_speed.y, -this->m_maxSpeed,
                                            runAcceleration * mult * dt);
  }
  if (keys[GLFW_KEY_S] && !m_gravityInfluence) {

    // move positive in the y axis, flying if there is not gravity influence
    float mult = 1.0f;
    if (this->m_speed.y < 0.0f)

      mult = 10.0f;

    this->m_speed.y = byPowerMath::approach(
        this->m_speed.y, this->m_maxSpeed / 2 + this->m_maxSpeed / 4,
        runAcceleration * mult * dt);
  }
  if (keys[GLFW_KEY_SPACE] && !keysProcessed[GLFW_KEY_SPACE] &&
      m_gravityInfluence && this->m_grounded) {
    // jump if is grounded
    keysProcessed[GLFW_KEY_SPACE] = true;
    this->m_speed.y = -80;
    this->m_grounded = false;
  } else if (keys[GLFW_KEY_SPACE] && !keysProcessed[GLFW_KEY_SPACE] &&
             m_gravityInfluence && this->m_wallTouch && !this->m_grounded) {
    // if the player is pushed against a wall and did not jump and is not
    // grounded can do a wall jump
    keysProcessed[GLFW_KEY_SPACE] = true;
    this->m_speed.y = -45;
    this->m_speed.x = 0.0f;
    this->m_speed.x = this->m_right ? -120 : 100;
  }
  if (!keys[GLFW_KEY_W] && !keys[GLFW_KEY_S] && !m_gravityInfluence) {
    // if therer is not gravity influence need to reduce his vel in the y axis
    this->m_speed.y = byPowerMath::approach(this->m_speed.y, 0, runReduce * dt);

  } else if (m_gravityInfluence && !this->m_grounded) {

    // affect the player by the gravity depending if is sliding in a wall or not
    // if have friction with a wall, meaning is pushing against a wall the
    // gravity will be lower

    bool get_positiveSpeed = this->m_speed.y >= 0.0f;

    float actualGravity =
        this->m_wallTouch && get_positiveSpeed ? gravity * 3 : gravity * 12;
    actualGravity = !this->m_pushed ? gravity * 15 : actualGravity;

    this->m_speed.y = byPowerMath::approach(
        this->m_speed.y, this->m_maxSpeed * 2.0f, actualGravity * dt);
  }
  // if the player is grounded therer is not gravity affecting him
  else if (m_gravityInfluence && this->m_grounded) {
    this->m_speed.y = 0.0f;
  }
}

void Player::move(float &dt) {

  if (this->m_pushed && this->m_grounded) {
    this->state = MOVE;
    this->m_direction.x = this->m_speed.x > 0 ? 1.0f : -1.0f;
  } else if (this->m_pushed && !this->m_grounded) {
    this->m_direction.x = this->m_speed.x > 0 ? 1.0f : -1.0f;

  }

  else if (!this->m_pushed && this->m_grounded ||
           this->m_speed.y == 0.0f && !this->m_pushed)
    this->state = IDLE;

  if (this->m_speed.y > 0.0f && !this->m_grounded)
    this->state = FALL;
  if (this->m_speed.y < 0.0f && !this->m_grounded)
    this->state = JUMP;

  if (this->m_wallTouch && !this->m_grounded)
    this->state = WALL_TOUCH;

  this->m_previusPos = this->m_position;
  this->m_position += this->m_speed;
  this->setPosInterpolation(dt);
}

void Player::m_anim(float &dt) {

  /*
  Depending of the state it changes the texture coords for the animation in the
  TextureAtlas.png
  */
  if (this->state == IDLE) {
    if (this->m_texCoords.position.y > 0.0f) {
      this->m_texCoords.position.y = 0.0f;
      this->m_texCoords.position.x = 0.0f;
      this->m_animTime = 0.0f;
    }
    if (this->m_texCoords.position.x == 96.0f)
      this->m_texCoords.position.x = 0.0f;
    else if (this->m_animTime >= this->m_animTimeLimit) {
      this->setNewTexOffset(glm::vec2(32.0f, 0.0f));
      this->m_animTime = 0.0f;
    } else
      this->m_animTime += dt * 5;
  } else if (this->state == MOVE) {
    if (this->m_texCoords.position.y < 16.0f ||
        this->m_texCoords.position.y > 16.0f) {
      this->m_texCoords.position.y = 0.0f;
      this->setNewTexOffset(glm::vec2(0.0f, 16.0f));
      this->m_texCoords.position.x = 0.0f;
      this->m_animTime = 0.0f;
    }
    if (this->m_texCoords.position.x > 224.0f)
      this->m_texCoords.position.x = 0.0f;
    else if (this->m_animTime >= this->m_animTimeLimit) {
      this->setNewTexOffset(glm::vec2(32.0f, 0.0f));
      this->m_animTime = 0.0f;
    } else
      this->m_animTime += dt * 5;
  } else if (this->state == JUMP) {

    if (this->m_texCoords.position.y < 48.0f ||
        this->m_texCoords.position.y > 48.0f ||
        this->m_texCoords.position.y == 48.0f &&
            m_texCoords.position.x > 0.0f) {
      this->m_texCoords.position.y = 48.0f;
      this->m_texCoords.position.x = 0.0f;
    }
  } else if (this->state == FALL) {
    if (this->m_texCoords.position.y < 48.0f ||
        this->m_texCoords.position.y > 48.0f ||
        this->m_texCoords.position.y == 48.0f &&
            this->m_texCoords.position.x > 32.0f ||
        this->m_texCoords.position.x < 32.0f) {
      this->m_texCoords.position.y = 48.0f;
      this->m_texCoords.position.x = 32.0f;
    }
  } else if (this->state == WALL_TOUCH) {
    if (this->m_texCoords.position.y < 64) {
      this->m_texCoords.position.y = 64.0f;
      this->m_texCoords.position.x = 0.0f;
    }
  }
}

void Player::m_onCollision() {
  if (this->m_col && this->m_bottom) {
    LOG("Player hurt a slime");

    if (this->m_speed.y > 0.0f) {
      this->m_speed.y = 0.0f;
      this->m_speed.y = -60.0f;
    }

    this->m_bottom = false;

  } else if (this->m_col && (this->m_right || this->m_left)) {
    LOG("Player was hurt by slime");
    if (this->m_speed.x > 0.0f || this->m_speed.x < 0.0f && this->m_pushed) {
      this->m_speed.x = 0.0f;
      this->m_speed.x = this->m_right ? -80.0f : 80.0f;
    } else
      this->m_speed.x = this->m_right ? -80.0f : 80.0f;

    this->m_right = false;
    this->m_left = false;
    this->m_color.w = 0.5f;
  } else if (this->m_col && this->m_top) {
    LOG("Player was hit by slime on top");
    if (this->m_speed.y < 0.0f)
      this->m_speed.y = 0.0f;
    this->m_top = false;
    this->m_color.w = 0.5f;
  }

  this->m_col = false;
}

void Player::m_updateTimer(float &dt) {
  if (this->m_hitTime >= this->m_hitTimeDur) {
    this->m_hitTime = 0.0f;
    this->m_updateTimers = false;
    this->m_color.w = 1.0f;
  } else if (this->m_hitTime < this->m_hitTimeDur && this->m_updateTimers) {

    this->m_hitTime += dt;
  }
}

void Player::m_setVisibilityHitbox(bool visibility) {
  this->m_showPlayerHitboxQuad = visibility;
}

bool Player::m_returnVisibilityHitbox() { return this->m_showPlayerHitboxQuad; }

std::string Player::m_getTagName() { return m_tagName; }
