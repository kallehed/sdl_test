#pragma once
#include "MovingRect.h"
#include "enums.h"

class Game;

class Shot : public MovingRect {
public:
	int _lives; // durability
	float _x_dir;
	float _y_dir;
	double _degrees_turned;
	float _speed;

	int _damage;

	TEX::TEX _image;

	// which entity created this shot(maybe don't hurt them?)?
	MovingRect* _owner;

	bool _set_new_owner = false; // used when, setting owner directly could make, for example, player not take hit of bullet, because player is owner
	MovingRect* _new_owner = nullptr;

	MOVING_RECT_TYPES get_moving_rect_type() const override;

	Shot(MovingRect* owner, int damage, float x, float y, float x_dir, float y_dir, float speed, int lives, TEX::TEX image = TEX::Bullet);

	void turn_according_to_dir();

	bool logic(Game& g) override;

	void draw(Game& g) override;

	void intersection(Game& g, float nx, float ny, MovingRect* e) override;

	
};
