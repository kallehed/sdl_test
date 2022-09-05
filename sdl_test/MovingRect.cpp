#include "MovingRect.h"
#include "Game.h"
#include "General.h"

void MovingRect::x_collision(Game& g) {
	int j_start = g._cam.convert_x_to_j(_x);
	int j_end = g._cam.convert_x_to_j((_x + _w - 0.01f)) + 1;
	int i_start = g._cam.convert_y_to_i(_y);
	int i_end = g._cam.convert_y_to_i((_y + _h - 0.01f)) + 1;

	float r_w = g._cam._fgrid;
	float r_h = g._cam._fgrid;
	for (int i = i_start; i < i_end; ++i)
	{
		for (int j = j_start; j < j_end; ++j)
		{
			TILE::TILE tile = g._tile_handler.get_tile_type(i, j);
			if (tile >= TILE::TRI_NE) {
				float r_x = j * r_w;
				float r_y = i * r_h;
				bool triangle = false;
				if (tile == TILE::TRI_NE) {
					// actually colliding
					if (TileHandler::intersection_tile<TILE::TRI_NE>(_x, _y, _w, _h, r_x, r_y, r_w, r_h))
					{
						// and, don't jump
						if (r_y + r_h >= _y + _h) {
							set_y(r_y - _h + _x - r_x);
							triangle = true;
						}
					}
				}
				else if (tile == TILE::TRI_SE) {
					if (TileHandler::intersection_tile<TILE::TRI_SE>(_x, _y, _w, _h, r_x, r_y, r_w, r_h)) {
						if (_y >= r_y) {
							set_y(r_y + -_x + r_x+r_w);
							triangle = true;
						}
					}
				}
				else if (tile == TILE::TRI_NW) {
					if (TileHandler::intersection_tile<TILE::TRI_NW>(_x, _y, _w, _h, r_x, r_y, r_w, r_h))  {
						if (_y+_h <= r_y+r_h) {
							set_y(r_y + r_h - _h - _x - _w + r_x);
							triangle = true;
						}
					}
				}
				else if (tile == TILE::TRI_SW) {
					if (TileHandler::intersection_tile<TILE::TRI_SW>(_x, _y, _w, _h, r_x, r_y, r_w, r_h)) {
						if (_y >= r_y) {
							set_y(r_y - r_x + _x + _w);
							triangle = true;
						}
					}
				}
				if (triangle) { goto GOTO_WHOLE; }
			}
		}
	}
GOTO_WHOLE:
	// whole, wíll only get here if no triangles were touched
	for (int i = i_start; i < i_end; ++i)
	{
		for (int j = j_start; j < j_end; ++j)
		{
			TILE::TILE tile = g._tile_handler.get_tile_type(i, j);
			if (tile > TILE::VOID && tile < TILE::TRI_NE) // whole
			{
				float r_x = j * r_w;
				float r_y = i * r_h;

				_x_vel = 0.f; // a b c a d c
				if (abs(_x + _w - r_x) < abs(r_x + r_w - _x)) {
					set_x(r_x - _w); // left side of r
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
	int j_start = g._cam.convert_x_to_j(_x);
	int j_end = g._cam.convert_x_to_j((_x + _w - 0.01f)) + 1;
	int i_start = g._cam.convert_y_to_i(_y);
	int i_end = g._cam.convert_y_to_i((_y + _h - 0.01f)) + 1;

	float r_w = g._cam._fgrid;
	float r_h = g._cam._fgrid;
	for (int i = i_start; i < i_end; ++i)
	{
		for (int j = j_start; j < j_end; ++j)
		{
			TILE::TILE tile = g._tile_handler.get_tile_type(i, j);
			if (tile >= TILE::TRI_NE && tile <= TILE::TRI_SW) {
				float r_x = j * r_w;
				float r_y = i * r_h;
				bool triangle = false;
				if (tile == TILE::TRI_NE) {
					if (TileHandler::intersection_tile<TILE::TRI_NE>(_x, _y, _w, _h, r_x, r_y, r_w, r_h))
					{
						if (_x >= r_x)
						{
							set_x(r_x + _y + _h - r_y);
							triangle = true;
						}
					}
				}
				else if (tile == TILE::TRI_SE) {
					if (TileHandler::intersection_tile<TILE::TRI_SE>(_x, _y, _w, _h, r_x, r_y, r_w, r_h)) {
						if (_x >= r_x) {
							set_x(r_x - _y + r_y + r_h);
							triangle = true;
						}
					}
				}
				else if (tile == TILE::TRI_NW) {
					if (TileHandler::intersection_tile<TILE::TRI_NW>(_x, _y, _w, _h, r_x, r_y, r_w, r_h)) {
						if (_x + _w <= r_x+r_w) {
							set_x(r_x + r_w - _w - _y - _h + r_y);
							triangle = true;
						}
					}
				}
				else if (tile == TILE::TRI_SW) {
					if (TileHandler::intersection_tile<TILE::TRI_SW>(_x, _y, _w, _h, r_x, r_y, r_w, r_h)) {
						if (_x+_w <= r_x+r_w) {
							set_x(r_x + r_w - _w - r_y - r_h + _y);
							triangle = true;
						}
					}
				}
				if (triangle) { goto GOTO_WHOLE; }
			}
		}
	}
	// whole
GOTO_WHOLE:
	for (int i = i_start; i < i_end; ++i)
	{
		for (int j = j_start; j < j_end; ++j)
		{
			TILE::TILE tile = g._tile_handler.get_tile_type(i, j);
			if (tile > TILE::VOID && tile < TILE::TRI_NE) // whole
			{
				float r_x = j * r_w;
				float r_y = i * r_h;

				_y_vel = 0.f;
				if (abs(_y + _h - r_y) < abs(r_y + r_h - _y)) {
					set_y(r_y - _h); // up side of r
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

template <bool COLLIDE>
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

	set_x(x() + _x_vel * g._dt);
	if constexpr (COLLIDE) {
		x_collision(g);
	}

	set_y(y() + _y_vel * g._dt);
	if constexpr (COLLIDE) {
		y_collision(g);
	}
}

template void MovingRect::move_and_collide<true>(Game& g);
template void MovingRect::move_and_collide<false>(Game& g);

void MovingRect::change_x_vel(float change)
{
	_frame_x_vel_change += change;
}

void MovingRect::change_y_vel(float change)
{
	_frame_y_vel_change += change;
}

void MovingRect::insert_to_intersect(EntityHandler& entity_handler)
{
	entity_handler._quadtree.add_to_head(this);
}

// left top of movingrect going towards point
void MovingRect::go_towards(float x, float y, float speed)
{
	float nx, ny;
	General::normalize_vector_two_points(nx, ny, mid_x(), mid_y(), x, y);
	change_x_vel(speed * nx);
	change_y_vel(speed * ny);
}

float MovingRect::x_vel() const
{
	return _x_vel;
}

float MovingRect::y_vel() const
{
	return _y_vel;
}

MovingRect::~MovingRect()
{
}
