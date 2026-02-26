#pragma once
#include<glm/glm.hpp>
#include"entity/entity.h"
#include"debugLog/debugLog.h"

bool point_in_rect(glm::vec2 point, Rect& rect)
{
	return (point.x >= rect.pos.x && point.x <= rect.pos.x + rect.size.x &&
		point.y >= rect.pos.y && point.y <= rect.pos.y + rect.size.y);
}

bool rect_collision(Rect& a, Rect& b)
{
	
	return (a.pos.x < b.pos.x + b.size.x && a.pos.x + a.size.x > b.pos.x &&
		a.pos.y < b.pos.y + b.size.y && a.pos.y + a.size.y > b.pos.y);
} 