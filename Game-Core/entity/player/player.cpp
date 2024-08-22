#include"player.h"
#include"debugLog/debugLog.h"

Player::Player(int& renderIndex, glm::vec3 position, glm::vec2 size,
	glm::vec4 color, float texSlot, glm::vec2 texPos,
	glm::vec2 texSize)
	:Entity2D_Instaciaded(renderIndex,position,size,color,texSlot,texPos,texSize)
{

}

Player::~Player() 
{}

Rect Player::m_getEntityRect()
{
	
	if (state == IDLE)
		return Rect(glm::vec2(this->m_position.x + 5, this->m_position.y + 3),
			glm::vec2(this->m_size.x - 8, this->m_size.y - 4),
			glm::vec2(this->m_previusPos.x + 5, this->m_previusPos.y +3),
			glm::vec2(5.0f,3.0f), glm::vec2(-8.0f,-4.0f));
	else if (state == MOVE)
		return Rect(this->m_position, this->m_size, this->m_previusPos);
}

void Player::m_animPlayer()
{

}