#pragma once

#include "MovingRect.h"
class PickupableShot : public MovingRect
{
public:
	int _lives = 1;

	MOVING_RECT_TYPES get_moving_rect_type() const override;

	PickupableShot(float x, float y, float x_vel, float y_vel);

	bool logic(Game& g) override;

	void draw(Game& g) override;

	void intersection(float nx, float ny, MovingRect* e) override;
};

