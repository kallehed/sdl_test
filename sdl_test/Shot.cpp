#include "Shot.h"
#include "Game.h"

MOVING_RECT_TYPES Shot::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::SHOT;
}

Shot::Shot(MovingRect* owner,float x, float y, float x_vel, float y_vel)
	: MovingRect(0, 0, 25.f, 25.f, 1.f), _owner(owner)
{
	set_x(x - get_half_w());
	set_y(y - get_half_h());
	_x_dir = x_vel;
	_y_dir = y_vel;
	

	_degrees_turned = atan2((double)y_vel, (double)x_vel) * (180.0/M_PI);
}

bool Shot::logic(Game& g)
{
	change_x_vel(_x_dir*_speed);
	change_y_vel(_y_dir*_speed);
	move_and_collide<false>(g);

	// collision with blocking tile == death

	auto res = General::get_blocking_tile_pos_in_area(g, get_x(), get_y(), get_w(), get_h());
	if (std::get<0>(res)) {
		auto pos = std::get<1>(res);
		g._tile_handler.hurt_tile(g, pos[2], pos[3]);
		_lives = 0;
	}
	if (_lives < 1)
	{
		delete this;
		return true;
	}

	return false;
}

void Shot::draw(Game& g)
{
	SDL_SetRenderDrawColor(g._renderer, 0, 255, 0, 255);

	SDL_Rect rect = { g._cam.convert_x((int)get_x()), g._cam.convert_y((int)get_y()),(int)get_w(),(int)get_h() };
	//SDL_RenderFillRect(g._renderer, &rect);

	SDL_RenderCopyEx(g._renderer, g._textures[TEX::Bullet], NULL, &rect, _degrees_turned, NULL, SDL_FLIP_NONE);
}

void Shot::intersection(float nx, float ny, MovingRect* e)
{
	switch (e->get_moving_rect_type()) {
	case MOVING_RECT_TYPES::FIRE_MAGIC:
	{
		_x_dir = nx; // change direction
		_y_dir = ny;
		_degrees_turned = atan2((double)ny, (double)nx) * (180.0 / M_PI);
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
