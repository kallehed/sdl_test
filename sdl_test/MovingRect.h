#pragma once

#include "Rect.h"
#include "TileHandler.h"
#include "enums.h"


class EntityHandler;
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

	void go_towards(float x, float y, float speed);
public:
	void change_x_vel(float change);
	void change_y_vel(float change);
	
	virtual void insert_to_intersect(EntityHandler& entity_handler); // real
	virtual void intersection(Game& g, float nx, float ny, MovingRect* e) = 0;
	virtual MOVING_RECT_TYPES get_moving_rect_type() const = 0;
	virtual void draw(Game& g) = 0;
	virtual bool logic(Game& g) = 0;

	float x_vel() const;
	float y_vel() const;

	virtual ~MovingRect();
};
