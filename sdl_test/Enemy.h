#pragma once

#include "MovingRect.h"
#include <vector>

class Enemy : public MovingRect {
public:
	bool _active = false;
	float _active_timer = 0;

	float _activation_radius_squared; // if less than this => activate
	float _deactivation_radius_squared; // if more than this => can be deactivated

	float _active_time;
	
	std::vector<std::array<int, 2>> _walk_path; // contains
	int _path_progress = 0;

	int _max_hp;
	int _hp;

	Enemy(float x, float y,float w, float h, int max_hp, float activation_radius, float deactivation_radius, float active_time);

	bool logic(Game& g);

	void make_active();

	void make_idle();

	void go_towards_player(Game& g, float speed);

	bool in_radius_squared_of_player(Game& g, float radius_squared);

	virtual void idle_logic(Game& g) = 0;

	virtual void active_logic(Game& g) = 0;

	virtual void take_damage(int damage = 1) = 0;
protected:
	void new_walk_path(Game& g);
};
