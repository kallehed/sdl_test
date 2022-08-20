#pragma once
#include "MovingRect.h"
class BossBody : public MovingRect
{
public:
	int _hp = 100;
	float _x_dir = 1.f;
	float _y_dir = 0.f;
	float _radians_turned = 0.f;

	int _damage = 5;

	BossBody* _back = nullptr;
	BossBody* _next;
	
	int _more_to_add;

	MOVING_RECT_TYPES get_moving_rect_type() const override;

	BossBody(float x, float y, int more_to_add = 6, BossBody* next = nullptr);

	bool logic(Game& g) override;

	void draw(Game& g) override;

	void intersection(Game& g, float nx, float ny, MovingRect* e) override;

	void take_damage(Game& g, int damage);
};

