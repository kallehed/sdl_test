#pragma once

#include "Enemy.h"

class EnemyDash : public Enemy {
private:
	ENEMY_DASH_STATE _state = ENEMY_DASH_STATE::WAITING;

	bool _idle_state = false;
	float _timer = 0.f;
	float _idle_x_dir = 0.f;
	float _idle_y_dir = 0.f;

	float _dash_x  = 0.f;
	float _dash_y = 0.f;

	float _active_timer = 500.f*General::randf01();

	float _hurt_timer = 0.f;

	float _get_away_distance_squared; // defined in cstr
	float _get_closer_distance_squared;

	static constexpr float _idle_speed = 0.0018f;
	static constexpr float _active_basic_speed = 0.0015f;

public:
	MOVING_RECT_TYPES get_moving_rect_type() const override final;

	EnemyDash(float x, float y);
	void draw(Game& g) override;
	void idle_logic(Game& g);
	void take_damage(Game& g, int damage = 1) override final;
	void active_logic(Game& g);
	void stay_in_range_of_player(Game& g);
};

