#pragma once
#include "MovingRect.h"

class Game;

class ShotEnemy : public MovingRect {
public:
	int _lives = 1;
	float _x_dir;
	float _y_dir;
	float _speed = 0.05f;

	MOVING_RECT_TYPES get_moving_rect_type() const override;

	ShotEnemy(float x, float y, float x_vel, float y_vel);

	bool logic(Game& g);

	void hit_something();

	void draw(Game& g);

	void intersection(float nx, float ny, MovingRect* e) override;


};
