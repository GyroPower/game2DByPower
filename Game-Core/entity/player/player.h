#pragma once 
#include "entity/entity.h"

class Player : public Entity2D_Instaciaded {
private:


public:
	Player(int& renderIndex, glm::vec3 position = glm::vec3(0.0f), glm::vec2 size = glm::vec2(10.0f),
		glm::vec4 color = glm::vec4(1.0f), float texSlot = -1.0f, glm::vec2 texPos = glm::vec2(0.0f),
		glm::vec2 texSize = glm::vec2(16.0f));

	~Player();

	void m_animPlayer();

	Rect m_getEntityRect();
};
