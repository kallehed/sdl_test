#pragma once
#include "MovingRect.h"

class Buyable : public MovingRect
{
public:
	int _onetime_index;

	int _cost; // cost to buy in coins

	BUYABLE_TYPE::_ _type;

	// text to display ALL THE TIME
	const char* _descriptive_text = "ERROR_DESCRIPTIVE_TEXT";

	bool _show_e_sign = false; // "Press e"

	// EITHER true when player opens chest
	bool _transaction_succeded = false; // display buy text
	bool _transaction_failed = false; // display fail text

	int _text_scale = 2;

	MOVING_RECT_TYPES get_moving_rect_type() const;

	Buyable(int onetime_index, int cost, BUYABLE_TYPE::_ type, float x, float y);

	bool logic(Game& g);

	void draw(Game& g);

	void intersection(Game& g, float nx, float ny, MovingRect* e);
};

