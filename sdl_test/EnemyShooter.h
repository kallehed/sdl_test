#pragma once

#include "Enemy.h"

class EnemyShooter final : public Enemy {
private:
	float _shoot_timer = 0.f;
public:
	MOVING_RECT_TYPES get_moving_rect_type() const override final;

	EnemyShooter(float x, float y);
	void draw(Game& g) override;
	void active_logic(Game& g) override;
};

