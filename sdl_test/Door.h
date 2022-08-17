#pragma once
#include "MovingRect.h"

class Door : public MovingRect
{
public:
	int _onetime_index;

	// text to display ALL THE TIME
	const char* _descriptive_text = "Oh hey";

	bool _show_e_sign = false; // "Press e"

	bool _opened = false;
	bool _opened_text = false;

	MOVING_RECT_TYPES get_moving_rect_type() const;

	Door(float x, float y, int onetime_index);

	bool logic(Game& g);

	void draw(Game& g);

	void intersection(Game& g, float nx, float ny, MovingRect* e);
};
