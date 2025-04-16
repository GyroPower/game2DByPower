#include"player.h"
#include"debugLog/debugLog.h"
#include"mathFuncs/mathFuncs.h"

Player::Player(int* renderIndex, glm::vec3 position, glm::vec2 size,
	glm::vec4 color, glm::vec2 posOffsetRect, glm::vec2 sizeOffsetRect, float texSlot, glm::vec2 texPos,
	glm::vec2 texSize)
	:Entity2D_Instaciaded(renderIndex, position, size, color, posOffsetRect, sizeOffsetRect, texSlot,
		texPos, texSize), m_showPlayerHitboxQuad(false), m_hitTime(0.0f), m_hitTimeDur(3.0f)
{
	this->m_tagName = "player";
	this->m_color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	this->m_gravityInfluence = true;
}

Player::~Player() 
{}

Rect Player::m_getEntityRect()
{
	
	if (state == IDLE || state == FALL || state == JUMP || state == WALL_TOUCH)
	{
		if (this->m_direction.x == 1.0f || this->m_direction.x == 0.0f)
			return Rect(glm::vec2(this->m_position.x, this->m_position.y),
				glm::vec2(this->m_size.x, this->m_size.y),
				glm::vec2(this->m_previusPos.x, this->m_previusPos.y),
				this->m_posOffsetRect, this->m_sizeOffsetRect);

		float offset = (this->m_posOffsetRect.x + this->m_sizeOffsetRect.x) * -1;

		return Rect(glm::vec2(this->m_position.x, this->m_position.y),
			glm::vec2(this->m_size.x, this->m_size.y),
			glm::vec2(this->m_previusPos.x, this->m_previusPos.y),
			glm::vec2(offset, this->m_posOffsetRect.y), this->m_sizeOffsetRect);



	}
	else if (state == MOVE)
	{
		if (this->m_direction.x == 1.0f || this->m_direction.x == 0.0f)
			return Rect(glm::vec2(this->m_position.x, this->m_position.y),
				glm::vec2(this->m_size.x, this->m_size.y),
				glm::vec2(this->m_previusPos.x, this->m_previusPos.y),
				glm::vec2(this->m_posOffsetRect.x, this->m_posOffsetRect.y + 2), glm::vec2(this->m_sizeOffsetRect.x, this->m_sizeOffsetRect.y - 2));

		float offset = (this->m_posOffsetRect.x + this->m_sizeOffsetRect.x) * -1;


		return Rect(glm::vec2(this->m_position.x, this->m_position.y),
			glm::vec2(this->m_size.x, this->m_size.y),
			glm::vec2(this->m_previusPos.x, this->m_previusPos.y),
			glm::vec2(offset, this->m_posOffsetRect.y + 2),
			glm::vec2(-this->m_posOffsetRect.x, this->m_sizeOffsetRect.y - 2));
	}
	
}

void Player::move(float& dt)
{
	

	if (this->m_pushed && this->m_grounded)
	{
		this->state = MOVE;
		this->m_direction.x = this->m_speed.x > 0 ? 1.0f : -1.0f;
	}
	else if (this->m_pushed && !this->m_grounded)
	{
		this->m_direction.x = this->m_speed.x > 0 ? 1.0f : -1.0f;
		

	}

	else if (!this->m_pushed && this->m_grounded || this->m_speed.y == 0.0f && !this->m_pushed)
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

void Player::m_anim(float& dt)
{

	/*
	Depending of the state it changes the texture coords for the animation in the TextureAtlas.png
	*/
	if (this->state == IDLE)
	{
		if (this->m_texCoords.position.y > 0.0f)
		{
			this->m_texCoords.position.y = 0.0f;
			this->m_texCoords.position.x = 0.0f;
			this->m_animTime = 0.0f;
		}
		if (this->m_texCoords.position.x == 96.0f)
			this->m_texCoords.position.x = 0.0f;
		else if (this->m_animTime >= this->m_animTimeLimit)
		{
			this->setNewTexOffset(glm::vec2(32.0f, 0.0f));
			this->m_animTime = 0.0f;
		}
		else
			this->m_animTime += dt * 5;
	}
	else if (this->state == MOVE)
	{
		if (this->m_texCoords.position.y < 16.0f || this->m_texCoords.position.y > 16.0f)
		{
			this->m_texCoords.position.y = 0.0f;
			this->setNewTexOffset(glm::vec2(0.0f, 16.0f));
			this->m_texCoords.position.x = 0.0f;
			this->m_animTime = 0.0f;
		}
		if (this->m_texCoords.position.x > 224.0f)
			this->m_texCoords.position.x = 0.0f;
		else if (this->m_animTime >= this->m_animTimeLimit)
		{
			this->setNewTexOffset(glm::vec2(32.0f, 0.0f));
			this->m_animTime = 0.0f;
		}
		else
			this->m_animTime += dt * 5;
	}
	else if (this->state == JUMP)
	{
		
		if (this->m_texCoords.position.y < 48.0f || this->m_texCoords.position.y > 48.0f || this->m_texCoords.position.y == 48.0f && m_texCoords.position.x > 0.0f )
		{
			this->m_texCoords.position.y = 48.0f;
			this->m_texCoords.position.x = 0.0f;
		}
	}
	else if (this->state == FALL)
	{
		if (this->m_texCoords.position.y < 48.0f || this->m_texCoords.position.y > 48.0f || this->m_texCoords.position.y == 48.0f &&
			this->m_texCoords.position.x > 32.0f || this->m_texCoords.position.x < 32.0f)
		{
			this->m_texCoords.position.y = 48.0f;
			this->m_texCoords.position.x = 32.0f;
		}
	}
	else if (this->state == WALL_TOUCH)
	{
		if (this->m_texCoords.position.y < 64)
		{
			this->m_texCoords.position.y = 64.0f;
			this->m_texCoords.position.x = 0.0f;
		}
	}
}

void Player::m_onCollision()
{
	if (this->m_col && this->m_bottom)
	{
		LOG("Player hurt a slime");
		
		if (this->m_speed.y > 0.0f)
		{
			this->m_speed.y = 0.0f;
			this->m_speed.y = -60.0f;
		}
		
		this->m_bottom = false;

	}
	else if (this->m_col && (this->m_right || this->m_left))
	{
		LOG("Player was hurt by slime");
		if (this->m_speed.x > 0.0f || this->m_speed.x < 0.0f && this->m_pushed)
		{
			this->m_speed.x = 0.0f;
			this->m_speed.x = this->m_right ? -80.0f : 80.0f;
		}
		else
			this->m_speed.x = this->m_right ? -80.0f : 80.0f;

		this->m_right = false;
		this->m_left = false;
		this->m_color.w = 0.5f;
	}
	else if (this->m_col && this->m_top)
	{
		LOG("Player was hit by slime on top");
		if (this->m_speed.y < 0.0f)
			this->m_speed.y = 0.0f;
		this->m_top = false;
		this->m_color.w = 0.5f;
	}

	this->m_col = false;
}

void Player::m_updateTimer(float& dt)
{
	if (this->m_hitTime >= this->m_hitTimeDur)
	{
		this->m_hitTime = 0.0f;
		this->m_updateTimers = false;
		this->m_color.w = 1.0f;
	}
	else if (this->m_hitTime < this->m_hitTimeDur && this->m_updateTimers)
	{
		 
		this->m_hitTime += dt;
	}
}

void Player::m_setVisibilityHitbox(bool visibility)
{
	this->m_showPlayerHitboxQuad = visibility;
}

bool Player::m_returnVisibilityHitbox()
{
	return this->m_showPlayerHitboxQuad;
}

std::string Player::m_getTagName()
{
	return m_tagName;
}