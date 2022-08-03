#pragma once
#include "MovingRect.h"
class Coin : public MovingRect
{
public:
	int _lives = 1;

	MOVING_RECT_TYPES get_moving_rect_type() const override;

	Coin(float x, float y, float x_vel, float y_vel);

	bool logic(Game& g) override;

	void draw(Game& g) override;

	void intersection(Game& g, float nx, float ny, MovingRect* e) override;
};

