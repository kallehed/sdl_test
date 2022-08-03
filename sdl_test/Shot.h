#pragma once
#include "MovingRect.h"
#include "enums.h"

class Game;

class Shot : public MovingRect {
public:
	int _lives = 1;
	float _x_dir;
	float _y_dir;
	double _degrees_turned;
	float _speed; //= 0.05f;

	TEX::TEX _image;

	// which entity created this shot(maybe don't hurt them?)?
	MovingRect* _owner;

	MOVING_RECT_TYPES get_moving_rect_type() const override;

	Shot(MovingRect* owner, float x, float y, float x_dir, float y_dir, float speed, TEX::TEX image = TEX::Bullet);

	void turn_according_to_dir();

	bool logic(Game& g) override;

	void draw(Game& g) override;

	void intersection(Game& g, float nx, float ny, MovingRect* e) override;

	
};
