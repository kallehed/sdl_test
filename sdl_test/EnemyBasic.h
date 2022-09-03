#pragma once

#include "Enemy.h"
#include "General.h"

class EnemyBasic : public Enemy {
private:
	float _active_timer = 3000.f * General::randf01();
	
	ENEMY_BASIC_STATE _state = ENEMY_BASIC_STATE::NORMAL;

public:
	MOVING_RECT_TYPES get_moving_rect_type() const override final;

	EnemyBasic(float x, float y);
	void draw(Game& g) override final;
	void take_damage(Game& g, int damage = 5) override final;
	void active_logic(Game& g) override final;
};

