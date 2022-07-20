#pragma once
#include "MovingRect.h"

class Game;

class Player : public MovingRect {
public:
	int _max_hp = 50;
	int _hp = _max_hp;

	MOVING_RECT_TYPES get_moving_rect_type() const override;
	Player();
	void logic(Game& g);

	void possibly_shoot(Game& g);


	void enemy_intersection(float nx, float ny);

	void draw(Game& g);

	void intersection(float nx, float ny, MovingRect* e) override;
};
