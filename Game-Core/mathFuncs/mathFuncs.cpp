#include"mathFuncs.h"

namespace byPowerMath
{
	float lerp(float first, float second, float time)
	{
		return first * (1 - time) + second * time ;
	}

	glm::vec3 lerp(glm::vec3 first, glm::vec3 second, float time)
	{
		glm::vec3 result(0.0f);
		result.x = lerp(first.x, second.x, time);
		result.y = lerp(first.y, second.y, time);
		result.z = lerp(first.z, second.z, time);

		return result;
	}

	int sign(float num)
	{
		if (num > 0.0f)
			return 1;
		else
			return -1;
	}

	float min(float a, float b)
	{
		if (a < b)
			return a;
		return b;
	}

	float max(float a, float b)
	{
		if (a > b)
			return a;
		return b;
	}

	float approach(float current, float target, float increase)
	{
		if (current < target)
			return min(current + increase, target);
		return max(current - increase, target);
	}
}