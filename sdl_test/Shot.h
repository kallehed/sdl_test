#pragma once
#include "MovingRect.h"

class Game;

class Shot : public MovingRect {
public:
	int _lives = 1;
	float _x_dir;
	float _y_dir;
	float _speed = 0.05f;

	// which entity created this shot(maybe don't hurt them?)?
	MovingRect* _owner;

	MOVING_RECT_TYPES get_moving_rect_type() const override;

	Shot(MovingRect* owner, float x, float y, float x_vel, float y_vel);

	bool logic(Game& g) override;

	void draw(Game& g) override;

	void intersection(float nx, float ny, MovingRect* e) override;

	
};
