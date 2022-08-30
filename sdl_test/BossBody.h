#pragma once
#include "MovingRect.h"
#include "General.h"

class BossBody : public MovingRect
{
public:
	static int _onetime_index;

	static constexpr int _largest_more_to_add = 6;
	static constexpr int _max_hp = 70;
	//static constexpr int _max_hp = 20;

	int _hp = _max_hp;
	float _x_dir = 1.f;
	float _y_dir = 0.f;
	float _radians_turned = General::pi();

	int _damage = 5;

	BossBody* _back = nullptr;
	BossBody* _next;
	
	int _more_to_add;

	float _timer = -420.f; // if -420.f, can activate timer for speedy_mode, if head

	bool _speedy_mode = false;

	Uint8 _color; // color for speedy mode to multiply colors by 

	MOVING_RECT_TYPES get_moving_rect_type() const override;

	// for head
	BossBody(int onetime_index, float x, float y);

	// for bodies
	BossBody(float x, float y, int more_to_add, BossBody* next = nullptr);

	bool logic(Game& g) override;

	void activate_speedy_mode(float time, Uint8 color);

	void draw(Game& g) override;

	void intersection(Game& g, float nx, float ny, MovingRect* e) override;

	void take_damage(Game& g, int damage);
};

