#include "MovingRect.h"
#include "Game.h"
#include "General.h"

void MovingRect::x_collision(Game& g) {
	float x = get_x();
	float y = get_y();
	float w = get_w();
	float h = get_h();
	int j_start = g._cam.convert_x_to_j(x);
	int j_end = g._cam.convert_x_to_j((x + w - 0.01f)) + 1;
	int i_start = g._cam.convert_y_to_i(y);
	int i_end = g._cam.convert_y_to_i((y + h - 0.01f)) + 1;

	float r_w = g._cam._fgrid;
	float r_h = g._cam._fgrid;
	for (int i = i_start; i < i_end; ++i)
	{
		for (int j = j_start; j < j_end; ++j)
		{
			TILE tile = g._tile_handler.get_tile_type(i, j);
			if (tile >= TILE::TRI_NE) {
				float r_x = j * r_w;
				float r_y = i * r_h;
				if (tile == TILE::TRI_NE) {
					// actually colliding, and not jump
					if (r_y + x - r_x <= y + h) {
						if (r_y + r_h >= y + h) {
							set_y(r_y - h + x - r_x);
							goto GOTO_WHOLE;
						}
					}
				}
				else if (tile == TILE::TRI_SE) {
					if ( x <= r_x + r_w - (y-r_y)) {
						if (y >= r_y) {
							set_y(r_y + -x + r_x+r_w);
						}
					}
				}
			}
		}
	}
GOTO_WHOLE:
	// whole, wíll only get here if no triangles were touched
	for (int i = i_start; i < i_end; ++i)
	{
		for (int j = j_start; j < j_end; ++j)
		{
			TILE tile = g._tile_handler.get_tile_type(i, j);
			if (tile > TILE::VOID && tile < TILE::TRI_NE) // whole
			{
				float r_x = j * r_w;
				float r_y = i * r_h;

				_x_vel = 0.f; // a b c a d c
				if (abs(x + w - r_x) < abs(r_x + r_w - x)) {
					set_x(r_x - w); // left side of r
				}
				else {
					set_x(r_x + r_w); // right side of r
				}
				return;
			}
		}
	}
}

void MovingRect::y_collision(Game& g) {
	float x = get_x();
	float y = get_y();
	float w = get_w();
	float h = get_h();
	int j_start = g._cam.convert_x_to_j(x);
	int j_end = g._cam.convert_x_to_j((x + w - 0.01f)) + 1;
	int i_start = g._cam.convert_y_to_i(y);
	int i_end = g._cam.convert_y_to_i((y + h - 0.01f)) + 1;

	float r_w = g._cam._fgrid;
	float r_h = g._cam._fgrid;
	for (int i = i_start; i < i_end; ++i)
	{
		for (int j = j_start; j < j_end; ++j)
		{
			TILE tile = g._tile_handler.get_tile_type(i, j);
			if (tile == TILE::TRI_NE) {
				float r_x = j * r_w;
				float r_y = i * r_h;
				if (r_y + (x - r_x) <= y + h) // actually colliding
				{
					if (x >= r_x)
					{
						set_x(r_x + w + y - r_y);
						goto GOTO_WHOLE;
					}
				}
			}
			else if (tile == TILE::TRI_SE) {
				float r_x = j * r_w;
				float r_y = i * r_h;
				if (x <= r_x + r_w - (y - r_y)) {
					if (x >= r_x) {
						set_x(r_x - y + r_y + r_h);
					}
				}
			}
		}
	}
	// whole
GOTO_WHOLE:
	for (int i = i_start; i < i_end; ++i)
	{
		for (int j = j_start; j < j_end; ++j)
		{
			TILE tile = g._tile_handler.get_tile_type(i, j);
			if (tile > TILE::VOID && tile < TILE::TRI_NE) // whole
			{
				float r_x = j * r_w;
				float r_y = i * r_h;

				_y_vel = 0.f;
				if (abs(y + h - r_y) < abs(r_y + r_h - y)) {
					set_y(r_y - h); // up side of r
				}
				else {
					set_y(r_y + r_h); // down side of r
				}
				return;
			}
		}
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
	_x_vel = fmax(-max_abs_vel, fmin(_x_vel, max_abs_vel));
	_y_vel = fmax(-max_abs_vel, fmin(_y_vel, max_abs_vel));

	set_x(get_x() + _x_vel * g._dt);
	x_collision(g);

	set_y(get_y() + _y_vel * g._dt);
	y_collision(g);
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
