#pragma once
#include "MovingRect.h"

class Game;

class Npc : public MovingRect
{
public:
	MOVING_RECT_TYPES get_moving_rect_type() const override final;

	Npc(float x, float y);

	bool logic(Game& g) override;

	void draw(Game& g) override;

	void intersection(Game& g, float nx, float ny, MovingRect* e) override;
};

