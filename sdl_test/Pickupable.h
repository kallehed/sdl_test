#pragma once

#include "MovingRect.h"
#include "enums.h"

class Game;

class Pickupable : public MovingRect
{
public:
	bool _picked_up = false;

	TEX::TEX _texture;

	PICKUPABLE_TYPE _type;

	MOVING_RECT_TYPES get_moving_rect_type() const override;

	Pickupable(PICKUPABLE_TYPE type, float x, float y, float x_vel, float y_vel);

	bool logic(Game& g) override;

	void draw(Game& g) override;

	void intersection(Game& g, float nx, float ny, MovingRect* e) override;
};

