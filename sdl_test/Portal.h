#pragma once
#include "MovingRect.h"

class Game;

class Portal : public MovingRect
{
public:
	bool _player_close = false;

	MOVING_RECT_TYPES get_moving_rect_type() const override final;

	Portal(float x, float y);

	bool logic(Game& g) override;

	void draw(Game& g) override;

	void intersection(Game& g, float nx, float ny, MovingRect* e) override;

};

