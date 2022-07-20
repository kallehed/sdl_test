#include "Shot.h"
#include "Game.h"

MOVING_RECT_TYPES Shot::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::SHOT;
}

Shot::Shot(float x, float y, float x_vel, float y_vel) : MovingRect(0, 0, 20.f, 20.f, 1.f)
{
	set_x(x - get_half_w());
	set_y(y - get_half_h());
	_x_dir = x_vel;
	_y_dir = y_vel;
}

void Shot::logic(Game& g)
{
	change_x_vel(_x_dir*_speed);
	change_y_vel(_y_dir*_speed);
	move_without_colliding(g);
}

bool Shot::end_logic(Game& g)  // returns whether to remove self or not
{
	// collision with blocking tile
	if (General::get_blocking_tile_pos_in_area(g, get_x(), get_y(), get_w(), get_h()).first
		|| _lives < 1)
	{
		return true;
	}
	return false;
}

void Shot::hit_something()
{
	_lives -= 1;
}

void Shot::draw(Game& g)
{
	SDL_SetRenderDrawColor(g._renderer, 0, 255, 0, 255);

	SDL_Rect rect = { g._cam.convert_x((int)get_x()), g._cam.convert_y((int)get_y()),(int)get_w(),(int)get_h() };
	SDL_RenderFillRect(g._renderer, &rect);
}

void Shot::intersection(float nx, float ny, MovingRect* e)
{
	MOVING_RECT_TYPES e_type = e->get_moving_rect_type();

	if (e_type == MOVING_RECT_TYPES::SHOT)
	{
		float bounce_acc = 0.005f;

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);
	}
	else if (e_type == MOVING_RECT_TYPES::ENEMY)
	{
		this->hit_something();
	}

}
