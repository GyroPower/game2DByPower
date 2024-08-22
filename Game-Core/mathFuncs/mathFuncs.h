#pragma once
#include<glm/glm.hpp>

namespace byPowerMath {

	float lerp(float first, float second, float time);
	glm::vec3 lerp(glm::vec3 first, glm::vec3 second, float time);

	int sign(float num);

	float min(float a, float b);
	float max(float a, float b);
	float approach(float current, float target, float increase);
}