#pragma once

#include <array>

class Game;


class General {
public:

	static int mod(int a, int b);

	static float randf01();

	// takes two references, which are set to normalized vectors of two points, returns length
	static float normalize_vector_two_points(float& nx, float& ny, float x1, float y1, float x2, float y2);

	// returns nx, ny, length
	static std::array<float, 3> normalize_vector_two_points_arr(float x1, float y1, float x2, float y2);

	constexpr static float pi();

	// 2 * pi
	constexpr static float tau();

	static float radians_to_degrees(float radians);

	// make radian between 0 and tau (2 * pi)
	static float normalize_radian(float radian);

	static float decrease_absolute_value(float value, float decrease);

	template <typename T>
	static bool rect_intersection(T& a, T& b);

	template <typename T>
	static bool general_rect_intersection(T x1, T y1, T w1, T h1, T x2, T y2, T w2, T h2);
};

template <typename T>
bool General::rect_intersection(T& a, T& b)
{
	return General::general_rect_intersection<float>(a.x(), a.y(), a.w(), a.h(), b.x(), b.y(), b.w(), b.h());
}


template <typename T>
bool General::general_rect_intersection(T x1, T y1, T w1, T h1, T x2, T y2, T w2, T h2)
{
	return x1 + w1 >= x2 && x1 <= x2 + w2 && y1 + h1 >= y2 && y1 <= y2 + h2;
}