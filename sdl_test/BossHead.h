#pragma once
#include "MovingRect.h"

class BossHead : public MovingRect
{
public:
	int _lives = 40;
	float _x_dir;
	float _y_dir;
	double _degrees_turned;

	int _damage = 5;

	MOVING_RECT_TYPES get_moving_rect_type() const override;

	BossHead(float x, float y);

	bool logic(Game& g) override;

	void draw(Game& g) override;

	void intersection(Game& g, float nx, float ny, MovingRect* e) override;
};

