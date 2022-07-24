#pragma once

#include "Rect.h"
#include "TileHandler.h"
class Game;

enum class MOVING_RECT_TYPES : char {
	PLAYER,
	ENEMY,
	SHOT,
	COIN,
	BOMB,
	EXPLOSION,
	FIRE_MAGIC,
	TOTAL
};


class MovingRect : public Rect {
private:
	void x_collision(TILE tile, float r_x, float r_y, float r_w, float r_h);
	void y_collision(TILE tile, float r_x, float r_y, float r_w, float r_h);
	float _frame_x_vel_change = 0.f;
	float _frame_y_vel_change = 0.f;
	float _friction;
	float _x_vel = 0.f;
	float _y_vel = 0.f;
protected:
	MovingRect(float x, float y, float w, float h, float friction);
	void move_and_collide(Game& g);
	void move_without_colliding(Game& g);
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
