#pragma once

#include "MovingRect.h"
#include <vector>

class Enemy : public MovingRect {
public:
	bool _active = false;
	float _timer = 0; // for both idle and active

	bool _idle_state = false; // whether to walk or stay still
	float _idle_x_dir = 0.f;
	float _idle_y_dir = 0.f;
	float _idle_speed;
	float _idle_still_time; // how long to stay still
	float _idle_walk_time; // how long to walk

	float _activation_radius_squared; // if less than this => activate
	float _deactivation_radius_squared; // if more than this => can be deactivated

	float _get_away_distance_squared;
	float _get_closer_distance_squared;

	float _active_time;
	
	std::vector<std::array<int, 2>> _walk_path; // contains
	int _path_progress = 0;

	float _active_basic_speed;

	int _max_hp;
	int _hp;

	int _damage = 5;

	float _hurt_timer = 0.f;

	Enemy(float x, float y,float w, float h, int max_hp, float activation_radius, float deactivation_radius, float active_time, float active_basic_speed, float idle_still_time, float idle_walk_time, float get_away_distance, float get_closer_distance, float idle_speed);

	bool logic(Game& g) override;

	void idle_logic(Game& g);

	void stay_in_range_of_player(Game& g);

	void set_active(bool active);

	void go_towards_player(Game& g, float speed);

	bool in_radius_squared_of_player(Game& g, float radius_squared);

	virtual void active_logic(Game& g) = 0;

	virtual void take_damage(Game& g, int damage = 5);
protected:
	void new_walk_path(Game& g);
public:
	void intersection(Game& g, float nx, float ny, MovingRect* e) override final;
};
