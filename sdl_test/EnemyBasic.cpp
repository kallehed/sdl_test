#include "EnemyBasic.h"
#include "Game.h"
#include <math.h>

const float EnemyBasic::_idle_speed = 0.001f;
const float EnemyBasic::_active_basic_speed = 0.0015f;

MOVING_RECT_TYPES EnemyBasic::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::ENEMY;
}

EnemyBasic::EnemyBasic(float x, float y) : Enemy(x, y, 20, 120.f, 150.f, 5000.f)
{

}

void EnemyBasic::idle_logic(Game& g)
{
	_timer += g._dt;
	if (!_idle_state) // wait for a moment still
	{
		if (_timer > 3000.f) {
			_idle_state = true;
			_timer = 0.f;

			float random_radians = ((float)M_PI * (rand() % 360)) / 180.f;
			_idle_x_dir = cos(random_radians);
			_idle_y_dir = sin(random_radians);
		}
	}
	else { // walk
		change_x_vel(_idle_speed * _idle_x_dir);
		change_y_vel(_idle_speed * _idle_y_dir);
		if (_timer > 2000.f) {
			_idle_state = false;
			_timer = 0.f;
		}
	}
}

void EnemyBasic::take_damage() {
	_hp -= 5;

	// possibly get scared
	int randint = rand() % 100;
	if (randint >= 80) // 20 % chance
	{
		if (randint >= 90) {
			_active_increased_speed = true;
			_timer = 0.f;
		}
		else {
			_active_scared = true;
			_timer = 0.f;
		}
		
	}
}

void EnemyBasic::active_logic(Game& g)
{
	if (_active_scared) // run away from player
	{ 
		go_towards_player(g, (-1.f) * _active_basic_speed);
		_timer += g._dt;
		if (_timer > 3000) {
			_active_scared = false;
		}
	} else
	if (_active_increased_speed) // run away from player
	{
		go_towards_player(g, _active_basic_speed*2.f);
		_timer += g._dt;
		if (_timer > 3000) {
			_active_increased_speed = false;
		}
	}
	else
	if (g._tile_handler.is_path_clear(g, get_x(), get_y(), g._entity_handler._p.get_x(), g._entity_handler._p.get_y()))
	{
		go_towards_player(g, _active_basic_speed);
		_walk_path.clear(); // clear path
	}
	else
	{
		// walk in path
		if (_walk_path.empty() || _path_progress >= _walk_path.size())
		{
			new_walk_path(g);
			if (_walk_path.empty())
			{
				go_towards_player(g, _active_basic_speed);
			}
		}
		else // move through A* path
		{ 
			// left upper point of tile
			float tile_x = _walk_path[_path_progress][1] * g._cam._fgrid;
			float tile_y = _walk_path[_path_progress][0] * g._cam._fgrid;
			// where the (middle point of this rect) should go
			float dst_x = tile_x + g._cam._fgrid / 2.f;
			float dst_y = tile_y + g._cam._fgrid / 2.f;
			go_towards(dst_x, dst_y, _active_basic_speed);

			// completely inside tile => go to next
			if (get_x() > tile_x && get_y() > tile_y &&
				get_x() + get_w() < tile_x + g._cam._grid && get_y() + get_h() < tile_y + g._cam._grid)
			{
				_path_progress += 1;
			}
		}
	}
}

void EnemyBasic::intersection(float nx, float ny, MovingRect* e)
{
	switch (e->get_moving_rect_type()) {
	case MOVING_RECT_TYPES::SHOT:
	{
		float bounce_acc = 0.05f;

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);

		take_damage();

		make_active(); // become active (aggressive)

		break;
	}
	case MOVING_RECT_TYPES::ENEMY:
	{
		float bounce_acc = 0.005f;

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);
		break;
	}
	case MOVING_RECT_TYPES::EXPLOSION:
	{
		float bounce_acc = 0.1f;

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);

		this->take_damage();
		this->make_active();
		break;
	}
	case MOVING_RECT_TYPES::FIRE_MAGIC:
	{
		float bounce_acc = 0.05f;

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);

		take_damage();
		make_active();

		break;
	}
	}
	
}
