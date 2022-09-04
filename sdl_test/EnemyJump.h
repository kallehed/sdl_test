#pragma once

#include "Enemy.h"

class EnemyJump : public Enemy {
private:
	ENEMY_JUMP_STATE _state = ENEMY_JUMP_STATE::WAITING;

	float _jump_x = 0.f; // dir + speed to jump
	float _jump_y = 0.f;

	float _jump_y_dif = 0.f; // how high above ground when jumping.
	float _jump_vel = 0.f; // jump velocity
	bool _bombed = false;

	float _active_timer = 500.f * General::randf01();

public:
	MOVING_RECT_TYPES get_moving_rect_type() const override final;

	EnemyJump(float x, float y);
	void draw(Game& g) override;
	void active_logic(Game& g);
};

