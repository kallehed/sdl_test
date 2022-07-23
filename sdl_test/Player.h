#pragma once
#include "MovingRect.h"

class Game;

class Player : public MovingRect {
public:
	int _max_hp = 50;
	int _hp = _max_hp;
	int _coins = 0;

	float _shoot_timer = 0.f;
	float _shoot_time = 400.f; // "reload" time

	MOVING_RECT_TYPES get_moving_rect_type() const override final;

	Player();

	bool logic(Game& g) override;

	void draw(Game& g) override;

	void intersection(float nx, float ny, MovingRect* e) override;
};
