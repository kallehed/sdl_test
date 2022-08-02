#pragma once

#include "Enemy.h"

class EnemyShooter : public Enemy {
private:
	bool _idle_state = false;
	float _timer = 0.f;
	float _idle_x_dir = 0.f;
	float _idle_y_dir = 0.f;

	float _shoot_timer = 0.f;

	float _get_away_distance_squared; // defined in cstr
	float _get_closer_distance_squared;

	static constexpr float _idle_speed = 0.0011f;
	static constexpr float _active_basic_speed = 0.0015f;

public:
	MOVING_RECT_TYPES get_moving_rect_type() const override;

	EnemyShooter(float x, float y);
	void draw(Game& g) override;
	void idle_logic(Game& g);
	void take_damage();
	void active_logic(Game& g);

	void intersection(float nx, float ny, MovingRect* e) override;
};

