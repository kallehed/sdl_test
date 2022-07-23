#pragma once
#include "MovingRect.h"
class Bomb : public MovingRect
{
public:
	int _lives = 2;
	float _detonation_timer = 2500.f;

	MOVING_RECT_TYPES get_moving_rect_type() const override;

	Bomb(float x, float y, float x_vel, float y_vel);

	bool logic(Game& g) override;

	void draw(Game& g) override;

	void intersection(float nx, float ny, MovingRect* e) override;
};