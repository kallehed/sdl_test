#pragma once

#include "Rect.h"
#include "TileHandler.h"
#include "enums.h"

class Game;
class Camera;

class MovingRect : public Rect {
private:
	void x_collision(Game& g);
	void y_collision(Game& g);
	float _frame_x_vel_change = 0.f;
	float _frame_y_vel_change = 0.f;
	float _friction;
	float _x_vel = 0.f;
	float _y_vel = 0.f;
protected:
	MovingRect(float x, float y, float w, float h, float friction);

	template <bool COLLIDE>
	void move_and_collide(Game& g);

	void change_x_vel(float change);
	void change_y_vel(float change);
	void go_towards(float x, float y, float speed);
public:
	
	virtual void intersection(float nx, float ny, MovingRect* e) = 0;
	virtual MOVING_RECT_TYPES get_moving_rect_type() const = 0;
	virtual void draw(Game& g) = 0;
	virtual bool logic(Game& g) = 0;

	float get_x_vel() const;
	float get_y_vel() const;
};
