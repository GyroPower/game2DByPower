#include"player.h"
#include"debugLog/debugLog.h"

Player::Player(int* renderIndex, glm::vec3 position, glm::vec2 size,
	glm::vec4 color, glm::vec2 posOffsetRect, glm::vec2 sizeOffsetRect, float texSlot, glm::vec2 texPos,
	glm::vec2 texSize)
	:Entity2D_Instaciaded(renderIndex,position,size,color,posOffsetRect,sizeOffsetRect,texSlot,texPos,texSize),m_showPlayerHitboxQuad(false)
{
	
}

Player::~Player() 
{}

Rect Player::m_getEntityRect()
{
	
	if (state == IDLE)
		return Rect(glm::vec2(this->m_position.x, this->m_position.y),
			glm::vec2(this->m_size.x, this->m_size.y),
			glm::vec2(this->m_previusPos.x, this->m_previusPos.y),
			this->m_posOffsetRect, this->m_sizeOffsetRect);
	else if (state == MOVE)
		return Rect(this->m_position, this->m_size, this->m_previusPos);
}

void Player::m_animPlayer()
{

}

void Player::m_setVisibilityHitbox(bool visibility)
{
	this->m_showPlayerHitboxQuad = visibility;
}

bool Player::m_returnVisibilityHitbox()
{
	return this->m_showPlayerHitboxQuad;
}