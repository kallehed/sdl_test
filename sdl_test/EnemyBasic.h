#pragma once

#include "Enemy.h"

class EnemyBasic : public Enemy {
private:
	bool _idle_state = false;
	float _timer = 0.f;
	float _idle_x_dir = 0.f;
	float _idle_y_dir = 0.f;

	static const float _idle_speed;
	static const float _active_basic_speed;

	bool _active_scared = false;
	bool _active_increased_speed = false;

public:
	MOVING_RECT_TYPES get_moving_rect_type() const override;

	EnemyBasic(float x, float y);
	void draw(Game& g) override;
	void idle_logic(Game& g);
	void take_damage();
	void active_logic(Game& g);

	void intersection(float nx, float ny, MovingRect* e) override;
};

