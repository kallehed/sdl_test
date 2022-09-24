#pragma once
#include "MovingRect.h"
class Bomb : public MovingRect
{
public:
	int _lives = 2;

	int _damage;
	float _area_factor;
	
	float _detonation_timer; // cstr

	MOVING_RECT_TYPES get_moving_rect_type() const override;

	Bomb(Game& g, float x, float y, float x_vel, float y_vel, int damage, float area_factor, float detonation_time=2000.f);

	bool logic(Game& g) override;

	void draw(Game& g) override;

	void intersection(Game& g,float nx, float ny, MovingRect* e) override;
};