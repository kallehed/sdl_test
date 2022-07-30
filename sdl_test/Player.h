#pragma once
#include "MovingRect.h"

enum class PLAYER_WEAPON : char {
	FIRE_MAGIC = 0,
	GUN,
	TOTAL
};

class Player : public MovingRect {
public:
	int _max_hp = 50;
	int _hp = _max_hp;
	int _coins = 0;

	PLAYER_WEAPON _left_weapon = PLAYER_WEAPON::FIRE_MAGIC;

	float _left_timer = 0.f;
	float _shoot_time = 400.f; // "reload" time

	// gun stuff
	//int _shots = 10;
	int _shots = 50;

	// bomb stuff
	int _bombs = 5;

	//bomb throw charge gauge
	float _bomb_throw_charge = 0.f;
	float _bomb_throw_max_charge = 1000.f;
	signed char _bomb_charge_direction = 1; // 1 = +, -1 = -
	bool _charging_bomb_throw = false;

	MOVING_RECT_TYPES get_moving_rect_type() const override final;

	Player();

	bool logic(Game& g) override;

	void draw(Game& g) override;

	void intersection(float nx, float ny, MovingRect* e) override;
};
