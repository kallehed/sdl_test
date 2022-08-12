#pragma once
#include "MovingRect.h"

class Chest : public MovingRect
{
public:
	int _onetime_index;

	bool _show_e_sign = false; // "Press e"

	// true when player opens chest
	bool _showing_text = false;

	MOVING_RECT_TYPES get_moving_rect_type() const;

	Chest(int onetime_index, float x, float y);

	bool logic(Game& g);

	void draw(Game& g);

	void intersection(Game& g, float nx, float ny, MovingRect* e);
};

