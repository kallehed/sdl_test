#include "General.h"
#include <math.h>
#include <cstdlib>

int General::mod(int a, int b) 
{
	return (a % b + b) % b;
}

// random float between 0 to 1
float General::randf01() {
	return ((float)rand()) / ((float)RAND_MAX);
}

// takes two references, which are set to normalized vectors of two points, returns length
float General::normalize_vector_two_points(float& nx, float& ny, float x1, float y1, float x2, float y2)
{
	float dx = x2 - x1;
	float dy = y2 - y1;
	float length = sqrtf(dx * dx + dy * dy);
	if (length == 0.f)
	{
		nx = 0.f;
		ny = 0.f;
		return 0.f;
	}
	nx = dx / length;
	ny = dy / length;
	return length;
}

std::array<float, 3> General::normalize_vector_two_points_arr(float x1, float y1, float x2, float y2)
{
	float dx = x2 - x1;
	float dy = y2 - y1;
	float length = sqrtf(dx * dx + dy * dy);
	if (length == 0.f)
	{
		return {0.f, 0.f, 0.f};
	}
	return { dx / length , dy / length, length };
}

constexpr float General::inv_sqrt_of_2()
{
	return 0.70710678118f;
}

float General::radians_to_degrees(float radians)
{
	return (180.f / General::pi()) * radians; // left side is compile-time probably
}

float General::normalize_radian(float radian)
{
	if (radian < 0.f)
	{
		return radian + General::tau();
	}
	else if (radian >= General::tau())
	{
		return radian - General::tau();
	}
	else
	{
		return radian;
	}
	
}

// assuming they have the same sign
float General::decrease_absolute_value(float value, float decrease)
{
	if (fabs(value) < fabs(decrease))
	{
		return 0.f;
	}
	else
	{
		return value - decrease;
	}
}
