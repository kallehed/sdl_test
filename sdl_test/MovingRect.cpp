#include "MovingRect.h"
#include "Game.h"
#include "General.h"

void MovingRect::x_collision(float r_x, float r_y, float r_w, float r_h) {
	_x_vel = 0.f; // a b c a d c
	if (abs(get_x() + get_w() - r_x) < abs(r_x + r_w - get_x())) {
		set_x(r_x - get_w()); // left side of r
	}
	else {
		set_x(r_x + r_w); // right side of r
	}
}

void MovingRect::y_collision(float r_x, float r_y, float r_w, float r_h) {
	_y_vel = 0.f;
	if (abs(get_y() + get_h() - r_y) < abs(r_y + r_h - get_y())) {
		set_y(r_y - get_h()); // up side of r
	}
	else {
		set_y(r_y + r_h); // down side of r
	}
}

MovingRect::MovingRect(float x, float y, float w, float h, float friction)
	:  Rect(x, y, w, h), _friction(friction) {
}

void MovingRect::move_and_collide(Game& g) // moves x and y based on velocity + collision with block_tiles
{
	// collision testing and moving

	float real_x_friction = _friction * g._dt * _x_vel;
	float real_y_friction = _friction * g._dt * _y_vel;

	_x_vel = General::decrease_absolute_value(_x_vel, real_x_friction);
	_y_vel = General::decrease_absolute_value(_y_vel, real_y_friction);

	_x_vel += _frame_x_vel_change * g._dt;
	_y_vel += _frame_y_vel_change * g._dt;
	_frame_x_vel_change = 0.f; // reset for next frame
	_frame_y_vel_change = 0.f;
	
	set_x(get_x() + _x_vel * g._dt);
	std::pair<bool, std::array<int, 2>> pos = General::get_blocking_tile_pos_in_area(g, get_x(), get_y(), get_w(), get_h());
	if (pos.first) { // will possibly set _x_vel to 0
		x_collision((float)pos.second[0], (float)pos.second[1], (float)g._cam._grid, (float)g._cam._grid);
	}

	set_y(get_y() + _y_vel * g._dt);
	pos = General::get_blocking_tile_pos_in_area(g, get_x(), get_y(), get_w(), get_h());
	if (pos.first) {
		y_collision((float)pos.second[0], (float)pos.second[1], (float)g._cam._grid, (float)g._cam._grid);
	}
}

void MovingRect::move_without_colliding(Game& g)
{
	float real_x_friction = _friction * g._dt * _x_vel;
	float real_y_friction = _friction * g._dt * _y_vel;

	_x_vel = General::decrease_absolute_value(_x_vel, real_x_friction);
	_y_vel = General::decrease_absolute_value(_y_vel, real_y_friction);

	_x_vel += _frame_x_vel_change * g._dt;
	_y_vel += _frame_y_vel_change * g._dt;
	_frame_x_vel_change = 0.f; // reset for next frame
	_frame_y_vel_change = 0.f;

	set_x(get_x() + _x_vel * g._dt);
	set_y(get_y() + _y_vel * g._dt);
}

void MovingRect::change_x_vel(float change)
{
	_frame_x_vel_change += change;
}

void MovingRect::change_y_vel(float change)
{
	_frame_y_vel_change += change;
}

// left top of movingrect going towards point
void MovingRect::go_towards(float x, float y, float speed)
{
	float nx, ny;
	General::normalize_vector_two_points(nx, ny, get_mid_x(), get_mid_y(), x, y);
	change_x_vel(speed * nx);
	change_y_vel(speed * ny);
}

void MovingRect::intersection(float nx, float ny, MovingRect* e)
{
	float bounce_acc = 0.005f;

	change_x_vel(bounce_acc * nx);
	change_y_vel(bounce_acc * ny);
}