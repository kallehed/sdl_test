#pragma once
#include <vector>
#include <array>
#include <climits>
class Game;

class AStarNode {
public:
	AStarNode* _parent = nullptr;
	int _G = INT_MAX; // walked path
	int _H = INT_MAX; // heuristic, manhattan distance
	int _F = INT_MAX; // G + H
	bool _closed = false;
	int _i = 0, _j = 0;

	void set_variables(int G, int H);

	static std::vector<std::array<int, 2>> generate_walk_path(Game& g, float x2, float y2, float x1, float y1);
};
