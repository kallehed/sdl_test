#pragma once

#include "Player.h"
#include "QuadTree.h"

class Game;

class EntityHandler {
public:
	Player _p;
	std::vector<MovingRect*> _entities;
	std::vector<MovingRect*> _draw_entities;
	std::vector<MovingRect*> _entities_to_add;
	Quadtree<MovingRect, 4, 5> _quadtree;
private:
	inline void do_logic(Game& g, std::vector<MovingRect*>& vec);
public:

	EntityHandler();
	void logic(Game& g);
	void draw(Game& g);
	void place_enemy(Game& g, int x, int y);
};
