#include "Shot.h"
#include "Game.h"

MOVING_RECT_TYPES Shot::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::SHOT;
}

Shot::Shot(MovingRect* owner, int damage, float x, float y, float x_dir, float y_dir, float speed, int lives, TEX::TEX image)
	: MovingRect(0, 0, 25.f, 25.f, 1.f), _owner(owner), _image(image), _speed(speed),
	_damage(damage), _lives(lives)
{
	set_x(x - half_w());
	set_y(y - half_h());
	_x_dir = x_dir;
	_y_dir = y_dir;
	turn_according_to_dir();
}

void Shot::turn_according_to_dir()
{

	_degrees_turned = atan2((double)_y_dir, (double)_x_dir) * (180.0 / M_PI);
}

bool Shot::logic(Game& g)
{

	if (_set_new_owner) {
		_owner = _new_owner;
		_set_new_owner = false;
	}

	_ASSERT(abs(_x_dir) <= 1.f);
	_ASSERT(abs(_y_dir) <= 1.f);

	change_x_vel(_x_dir*_speed);
	change_y_vel(_y_dir*_speed);

	move_and_collide<false>(g);

	// collision with blocking tile == death

	auto res = g._tile_handler.get_blocking_tile_pos_in_area(g, x(), y(), w(), h());
	if (std::get<0>(res)) {
		auto pos = std::get<1>(res);
		g._tile_handler.hurt_tile(g, pos[2], pos[3]);
		--_lives;
		TILE::TILE tile = std::get<2>(res);
		float tile_x = std::get<1>(res)[0];
		float tile_y = std::get<1>(res)[1];
		
		if (tile == TILE::BLOCK || tile == TILE::DESTRUCTABLE)
		{
			float dx = tile_x - x();
			float dy = tile_y - y();
			if (x_vel() >= 0.f) { // TODO: FIX ALL THIS WITH THE BOUNDING
					// top or left side or bottom of tile
				if (dx > dy) {
					// top side of tile
					_y_dir *= -1;
				}
				else {
					// left side of tile
					_x_dir *= -1;
				}
			}
		}

		//_x_dir *= -1; // TODO: MAKE IT BOUNCE THE RIGHT WAY
		//_y_dir *= -1;
		
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

	SDL_Rect rect = { g._cam.convert_x((int)x()), g._cam.convert_y((int)y()),(int)(w()*1.2f),(int)(h()*1.2f) };
	//SDL_RenderFillRect(g._renderer, &rect);

	SDL_RenderCopyEx(g._renderer, g._textures[_image], NULL, &rect, _degrees_turned, NULL, SDL_FLIP_NONE);
}

void Shot::intersection(Game& g, float nx, float ny, MovingRect* e)
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
		// *** CHANGE
		if (_owner != e)
		{
			_x_dir = nx;
			_y_dir = ny;
			turn_according_to_dir();
			_owner = e; // don't bounce into same thing next frame
		}
		break;
	}
	case MOVING_RECT_TYPES::ENEMY:
	case MOVING_RECT_TYPES::PLAYER:
	{
		if (_owner != e) { // DONT HURT SELF OVER OWNER!!!!
			--_lives; 

			_x_dir = nx;
			_y_dir = ny;
			turn_according_to_dir();
			_set_new_owner = true;
			_new_owner = e;
		}
		break;
	}
	case MOVING_RECT_TYPES::BOMB:
	{
		--_lives;
		break;
	}
	}

}
