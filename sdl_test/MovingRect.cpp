#include "MovingRect.h"
#include "Game.h"
#include "General.h"

void MovingRect::x_collision(TILE tile, float r_x, float r_y, float r_w, float r_h) {
	if ((int)tile < (int)TILE::TRI_NE || true) {
		_x_vel = 0.f; // a b c a d c
		if (abs(get_x() + get_w() - r_x) < abs(r_x + r_w - get_x())) {
			set_x(r_x - get_w()); // left side of r
		}
		else {
			set_x(r_x + r_w); // right side of r
		}
	}
	else if (tile == TILE::TRI_NE) {
		set_x(r_x + (get_y() - r_y) + get_w());
	}
}

void MovingRect::y_collision(TILE tile, float r_x, float r_y, float r_w, float r_h) {
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

	// limit
	// maximum velocity(otherwise *this will go through walls(also outofbounds!)) 
	constexpr float max_abs_vel = 1.7f; // 1.75 too high, 1.635 safe, 1.7 safe
	_x_vel = fmax(-max_abs_vel,fmin(_x_vel, max_abs_vel));
	_y_vel = fmax(-max_abs_vel, fmin(_y_vel, max_abs_vel));
	
	set_x(get_x() + _x_vel * g._dt);
	auto info = General::get_blocking_tile_pos_in_area(g, get_x(), get_y(), get_w(), get_h());
	if (std::get<0>(info)) { // will possibly set _x_vel to 0
		auto pos = std::get<1>(info);
		x_collision(std::get<2>(info), (float)pos[0], (float)pos[1], g._cam._fgrid, g._cam._fgrid);
	}

	set_y(get_y() + _y_vel * g._dt);
	info = General::get_blocking_tile_pos_in_area(g, get_x(), get_y(), get_w(), get_h());
	if (std::get<0>(info)) {
		auto pos = std::get<1>(info);
		x_collision(std::get<2>(info),(float)pos[0], (float)pos[1], g._cam._fgrid, g._cam._fgrid);
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

float MovingRect::get_x_vel() const
{
	return _x_vel;
}

float MovingRect::get_y_vel() const
{
	return _y_vel;
}
