#pragma once

#include "MovingRect.h"

class Particle : public MovingRect
{
public:
	float _time = 10000.f;
	float _timer = _time;
	SDL_Color _color;

	MOVING_RECT_TYPES get_moving_rect_type() const override;

	Particle(float x, float y, float x_vel, float y_vel, SDL_Color color);

	bool logic(Game& g) override;

	void draw(Game& g) override;

	void intersection(Game& g, float nx, float ny, MovingRect* e) override;
};