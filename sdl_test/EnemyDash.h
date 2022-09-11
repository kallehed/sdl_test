#pragma once

#include "Enemy.h"
#include "General.h"

class EnemyDash : public Enemy {
private:
	ENEMY_DASH_STATE _state = ENEMY_DASH_STATE::WAITING;

	float _dash_x  = 0.f; // dir + speed to jump
	float _dash_y = 0.f;

	float _active_timer = 500.f*General::randf01();

public:
	MOVING_RECT_TYPES get_moving_rect_type() const override final;

	EnemyDash(float x, float y);
	void draw(Game& g) override;
	void active_logic(Game& g);
};

