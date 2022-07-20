#pragma once

#include "Player.h"
#include "EnemyBasic.h"
#include "EnemyShooter.h"
#include "Shot.h"
#include "QuadTree.h"

class Game;

class EntityHandler {
public:
	Player _p;
	std::vector<MovingRect*> _entities;
	Quadtree<MovingRect, 4, 5> _quadtree;

	EntityHandler();
	void logic(Game& g);
	void draw(Game& g);
	void place_enemy(Game& g, int x, int y);
};
