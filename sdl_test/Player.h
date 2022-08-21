#pragma once
#include "MovingRect.h"
#include "enums.h"

class Player : public MovingRect {
public:
	int _max_hp = 40;
	int _hp = _max_hp;
	int _coins = 1000;

	// alive stuff
	bool _alive = true;
	bool _respawn = false; // flag for higher-ups to respond to
	float _death_timer = 0.f;

	// respawn stuff
	int _respawn_level = 0; 
	float _respawn_x = 1265.f;
	float _respawn_y = 2310.f;

	//invincibility frames stuff
	float _invi_time = 300.f;
	float _invi_timer = 0.f;

	// walk animation stuff
	bool _forward = true; // facing forward
	bool _right = true; // facing right
	float _walk_animation_timer = 0.f;

	// flash black when hit
	bool _took_damage_this_frame = false;

	L_WEAPON::_ _left_weapon = L_WEAPON::FIRE_MAGIC;

	//std::array<bool, L_WEAPON::TOTAL> _have_l_weapon = {true, false}; // play
	std::array<bool, L_WEAPON::TOTAL> _have_l_weapon = { true, true }; // dev

	float _left_timer = 0.f; // left weapon timer

	// Fire magic stuff
	float _fire_magic_max = 800.f;
	float _fire_magic_current = _fire_magic_max;
	float _fire_magic_cost = 225.f;
	float _fire_magic_increase = 0.175f;
	int _fire_magic_damage = 5;
	float _fire_magic_area_factor = 1.f;

	// gun stuff
	//int _shots = 0;
	int _shots = 50;
	int _shot_damage = 5;
	float _shoot_time = 600.f; // "reload" time

	// bomb stuff
	int _bombs = 5;
	float _bomb_area_factor = 1.f;
	int _bomb_damage = 10;

	//bomb throw charge gauge
	float _bomb_throw_charge = 0.f;
	float _bomb_throw_max_charge = 1000.f;
	signed char _bomb_charge_direction = 1; // 1 = +, -1 = -
	bool _charging_bomb_throw = false;

	// The ability to run: stuff
	//bool _ability_to_run = false; // play
	bool _ability_to_run = true; // dev
	float _run_max = 750.f;
	float _run_current = _run_max;
	float _run_recharge = 0.5f;
	float _run_cost = 0.75f;
	float _run_speed_boost = 1.6f;
	bool _using_run = false;


	MOVING_RECT_TYPES get_moving_rect_type() const override final;

	Player();

	bool logic(Game& g) override;

	void draw(Game& g) override;

	void take_damage(Game& g, int damage);

	void intersection(Game& g, float nx, float ny, MovingRect* e) override;
};
