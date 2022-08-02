#include "Shot.h"
#include "Game.h"

MOVING_RECT_TYPES Shot::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::SHOT;
}

Shot::Shot(MovingRect* owner,float x, float y, float x_vel, float y_vel, TEX::TEX image)
	: MovingRect(0, 0, 25.f, 25.f, 1.f), _owner(owner), _image(image)
{
	set_x(x - get_half_w());
	set_y(y - get_half_h());
	_x_dir = x_vel;
	_y_dir = y_vel;
	turn_according_to_dir();
}

void Shot::turn_according_to_dir()
{

	_degrees_turned = atan2((double)_y_dir, (double)_x_dir) * (180.0 / M_PI);
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

	SDL_Rect rect = { g._cam.convert_x((int)get_x()), g._cam.convert_y((int)get_y()),(int)(get_w()*1.4f),(int)(get_h()*1.4f) };
	//SDL_RenderFillRect(g._renderer, &rect);

	SDL_RenderCopyEx(g._renderer, g._textures[_image], NULL, &rect, _degrees_turned, NULL, SDL_FLIP_NONE);
}

void Shot::intersection(float nx, float ny, MovingRect* e)
{
	switch (e->get_moving_rect_type()) {
	case MOVING_RECT_TYPES::FIRE_MAGIC:
	case MOVING_RECT_TYPES::EXPLOSION:
	{
		_x_dir = nx; // change direction
		_y_dir = ny;
		turn_according_to_dir();
		_owner = nullptr; // NO ONE OWNS ME ANYMORE, FREEDOM!!!
		break;
	}
	case MOVING_RECT_TYPES::SHOT:
	{
		/*float bounce_acc = 0.005f; // doesn't do anything

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);*/
		// *** CHANGE
		{
			if (_owner != e) {
				_x_dir = nx;
				_y_dir = ny;
				turn_according_to_dir();
				//change_x_vel(_x_dir * _speed);
				//change_y_vel(_y_dir * _speed);
				_owner = e; // don't bounce into same thing next frame
			}
		}
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
