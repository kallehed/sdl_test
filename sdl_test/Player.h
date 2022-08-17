#pragma once
#include "MovingRect.h"
#include "enums.h"

class Player : public MovingRect {
public:
	int _max_hp = 50;
	int _hp = _max_hp;
	int _coins = 1000;

	//invincibility frames stuff
	float _invi_time = 500.f;
	float _invi_timer = 0.f;

	// walk animation stuff
	bool _forward = true; // facing forwawrd
	bool _right = true; // facing right
	float _walk_animation_timer = 0.f;

	PLAYER_WEAPON _left_weapon = PLAYER_WEAPON::FIRE_MAGIC;

	// Fire magic stuff
	float _fire_magic_max = 800.f;
	float _fire_magic_current = _fire_magic_max;
	float _fire_magic_cost = 225.f;
	float _fire_magic_increase = 0.175f;
	int _fire_magic_damage = 5;
	float _fire_magic_area_factor = 1.f;

	float _left_timer = 0.f;
	float _shoot_time = 400.f; // "reload" time

	// gun stuff
	//int _shots = 10;
	int _shots = 50;

	// bomb stuff
	int _bombs = 5;
	float _bomb_area_factor = 1.f;
	int _bomb_damage = 10;

	//bomb throw charge gauge
	float _bomb_throw_charge = 0.f;
	float _bomb_throw_max_charge = 1000.f;
	signed char _bomb_charge_direction = 1; // 1 = +, -1 = -
	bool _charging_bomb_throw = false;

	MOVING_RECT_TYPES get_moving_rect_type() const override final;

	Player();

	bool logic(Game& g) override;

	void draw(Game& g) override;

	void take_damage(Game& g, int damage = 1);

	void intersection(Game& g, float nx, float ny, MovingRect* e) override;
};
