#include "Shot.h"
#include "Game.h"

MOVING_RECT_TYPES Shot::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::SHOT;
}

Shot::Shot(MovingRect* owner,float x, float y, float x_vel, float y_vel)
	: MovingRect(0, 0, 20.f, 20.f, 1.f), _owner(owner)
{
	set_x(x - get_half_w());
	set_y(y - get_half_h());
	_x_dir = x_vel;
	_y_dir = y_vel;
}

bool Shot::logic(Game& g)
{
	change_x_vel(_x_dir*_speed);
	change_y_vel(_y_dir*_speed);
	move_without_colliding(g);

	// collision with blocking tile == death

	auto res = General::get_blocking_tile_pos_in_area(g, get_x(), get_y(), get_w(), get_h());
	if (res.first) {
		g._tile_handler.hurt_tile(res.second[2], res.second[3]);
		goto GOTO_destroy_self;
		//_lives = 0;
	}
	if (_lives < 1)
	{
	GOTO_destroy_self:
		delete this;
		return true;
	}

	return false;
}

void Shot::draw(Game& g)
{
	SDL_SetRenderDrawColor(g._renderer, 0, 255, 0, 255);

	SDL_Rect rect = { g._cam.convert_x((int)get_x()), g._cam.convert_y((int)get_y()),(int)get_w(),(int)get_h() };
	SDL_RenderFillRect(g._renderer, &rect);
}

void Shot::intersection(float nx, float ny, MovingRect* e)
{
	switch (e->get_moving_rect_type()) {
	case MOVING_RECT_TYPES::FIRE_MAGIC:
	{
		_x_dir = nx; // change direction
		_y_dir = ny;
		_owner = nullptr; // NO ONE OWNS ME ANYMORE, FREEDOM!!!
		break;
	}
	case MOVING_RECT_TYPES::SHOT:
	{
		/*float bounce_acc = 0.005f; // doesn't do anything

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);*/
		break;
	}
	case MOVING_RECT_TYPES::ENEMY:
	case MOVING_RECT_TYPES::PLAYER:
	{
		if (_owner == e) {
			break; // DONT HURT OWNER!!!!
		}
		_lives -= 1;
		break;
	}
	case MOVING_RECT_TYPES::BOMB:
	{
		_lives -= 1;
		break;
	}
	}

}
