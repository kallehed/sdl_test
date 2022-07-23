#pragma once
#include "MovingRect.h"
class Explosion : public MovingRect
{
public:
	float _explosion_timer = 0.f;
	
	bool _exploded = false;

	MOVING_RECT_TYPES get_moving_rect_type() const override;

	Explosion(float x, float y);

	bool logic(Game& g) override;

	void draw(Game& g) override;

	void intersection(float nx, float ny, MovingRect* e) override;
};