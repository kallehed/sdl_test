#pragma once
#include "MovingRect.h"
class Bonfire : public MovingRect
{
public:
	bool _player_close = false;

	bool _fire_animation = false;
	float _animation_timer = 0.f;

	static constexpr float _animation_time = 712.f;

	MOVING_RECT_TYPES get_moving_rect_type() const;

	Bonfire(float x, float y);

	bool logic(Game& g);

	void draw(Game& g);

	void intersection(Game& g, float nx, float ny, MovingRect* e);
};

