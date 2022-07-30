#include "EnemyShooter.h"
#include "Game.h"
#include "Shot.h"

const float EnemyShooter::_idle_speed = 0.0011f;
const float EnemyShooter::_active_basic_speed = 0.0015f;

MOVING_RECT_TYPES EnemyShooter::get_moving_rect_type() const
{
    return MOVING_RECT_TYPES::ENEMY;
}

EnemyShooter::EnemyShooter(float x, float y) : Enemy(x, y, 20, 200.f, 300.f, 5000.f)
{
	_get_away_distance_squared = powf(250.f - 50 * General::randf01(), 2);
	_get_closer_distance_squared = powf(300.f - 50 * General::randf01(), 2);
}

void EnemyShooter::draw(Game& g)
{
	SDL_SetRenderDrawColor(g._renderer, 150, 0, 225, 255);

	SDL_Rect rect = { g._cam.convert_x((int)get_x()), g._cam.convert_y((int)get_y()),(int)get_w(),(int)get_h() };
	SDL_RenderFillRect(g._renderer, &rect);
}

void EnemyShooter::idle_logic(Game& g)
{
	_timer += g._dt;
	if (!_idle_state) // wait for a moment still
	{
		if (_timer > 2000.f) {
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
		if (_timer > 3000.f) {
			_idle_state = false;
			_timer = 0.f;
		}
	}
}

void EnemyShooter::take_damage() {
	_hp -= 5;

	// possibly get scared
	/*int randint = rand() % 100;
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
	}*/
}

void EnemyShooter::active_logic(Game& g)
{
	_shoot_timer += g._dt;

	if (_shoot_timer > 750.f)
	{
		_shoot_timer = 0.f + 200.f*(General::randf01() - 0.5f);
		// shoot projectile "Shot"
		float shot_speed = 0.5f;
		float nx, ny;
		Player& p = g._entity_handler._p;
		General::normalize_vector_two_points(nx, ny, get_mid_x(), get_mid_y(), p.get_mid_x(), p.get_mid_y());

		float x_speed = nx * shot_speed;
		float y_speed = ny * shot_speed;
		auto new_shot = new Shot(this, get_mid_x(), get_mid_y(), x_speed, y_speed);
		g._entity_handler._entities_to_add.push_back(new_shot);

	}

	// check if distance to player is too far away => go closer
	float distance_to_player_squared;
	{
		float dx = get_mid_x() - g._entity_handler._p.get_mid_x();
		float dy = get_mid_y() - g._entity_handler._p.get_mid_y();
		distance_to_player_squared = dx * dx + dy * dy;
	}
	if (distance_to_player_squared > _get_closer_distance_squared)
	{
		// too far away
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
	else if (distance_to_player_squared < _get_away_distance_squared)
	{
		// get away
		go_towards_player(g, (-1.f) * _active_basic_speed);
	}
	else {
		// best bounds, stay still?
	}
}

void EnemyShooter::intersection(float nx, float ny, MovingRect* e)
{
	switch (e->get_moving_rect_type()) {
	case MOVING_RECT_TYPES::SHOT:
	{
		if (((Shot*)e)->_owner == this) { // can't shoot self(not instantly anyway)
			break;
		}
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
	case MOVING_RECT_TYPES::BOMB:
	{
		// do nothing? (Boring if enemies flee bomb)
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
