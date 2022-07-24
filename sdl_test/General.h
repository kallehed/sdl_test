#pragma once
#include <vector>
#include "TileHandler.h"

class Game;

class General {
public:

	static int mod(int a, int b);

	static float randf01();

	static float normalize_vector_two_points(float& nx, float& ny, float x1, float y1, float x2, float y2);

	static float decrease_absolute_value(float value, float decrease);

	template <typename T>
	static bool rect_intersection(T& a, T& b);

	template <typename T>
	static bool general_rect_intersection(T x1, T y1, T w1, T h1, T x2, T y2, T w2, T h2);

	// returns true if any blocking tiles, then gives pos(x, y), then (i, j), then TILE type, of tile
	static std::tuple<bool,std::array<int,4>,TILE> get_blocking_tile_pos_in_area(Game& g, float x, float y, float w, float h);

	static std::vector<std::array<int, 2>> generate_walk_path(Game& g, float x2, float y2, float x1, float y1);
};

template <typename T>
bool General::rect_intersection(T& a, T& b)
{
	return General::general_rect_intersection<float>(a.get_x(), a.get_y(), a.get_w(), a.get_h(), b.get_x(), b.get_y(), b.get_w(), b.get_h());
}


template <typename T>
bool General::general_rect_intersection(T x1, T y1, T w1, T h1, T x2, T y2, T w2, T h2)
{
	return x1 + w1 >= x2 && x1 <= x2 + w2 && y1 + h1 >= y2 && y1 <= y2 + h2;
}

class AStarNode {
public:
	AStarNode* _parent = nullptr;
	int _G = INT_MAX; // walked path
	int _H = INT_MAX; // heuristic, manhattan distance
	int _F = INT_MAX; // G + H
	bool _closed = false;
	int _i, _j;

	void set_variables(int G, int H);
};