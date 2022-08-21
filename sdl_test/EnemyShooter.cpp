#include "EnemyShooter.h"
#include "Game.h"
#include "Shot.h"
#include "Explosion.h"

MOVING_RECT_TYPES EnemyShooter::get_moving_rect_type() const
{
    return MOVING_RECT_TYPES::ENEMY;
}

EnemyShooter::EnemyShooter(float x, float y) : Enemy(x, y,45.f,45.f, 20, 200.f, 300.f, 5000.f)
{
	_get_away_distance_squared = powf(250.f - 50 * General::randf01(), 2);
	_get_closer_distance_squared = powf(300.f - 50 * General::randf01(), 2);
}

void EnemyShooter::draw(Game& g)
{
	SDL_SetRenderDrawColor(g._renderer, 150, 0, 225, 255);

	SDL_Rect rect = { g._cam.convert_x((int)x()), g._cam.convert_y((int)y()),(int)(_w*1.2f),(int)(_h*1.2f) };
	//SDL_RenderFillRect(g._renderer, &rect);

	draw_circle(g._renderer, rect.x + _w / 2, rect.y + _h * 0.9f, 20, { 0,0,0,67 });

	SDL_RendererFlip flip = (x_vel() > 0.f) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

	if (_hurt_timer > 0.f) {
		SDL_SetTextureColorMod(g._textures[TEX::Wizard], 100, 100, 100);
		_hurt_timer -= g._dt;
	}
	else {
		SDL_SetTextureColorMod(g._textures[TEX::Wizard], 255, 255, 255);
	}

	SDL_RenderCopyEx(g._renderer, g._textures[TEX::Wizard], NULL, &rect, NULL, NULL, flip);
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

void EnemyShooter::take_damage(Game& g, int damage) {
	_hp -= damage;
	_hurt_timer = 150.f;
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
		float speed = 0.028f;
		float nx, ny;
		Player& p = g._entity_handler._p;
		General::normalize_vector_two_points(nx, ny, mid_x(), mid_y(), p.mid_x(), p.mid_y());

		auto new_shot = new Shot(this, 5, mid_x(), mid_y(), nx, ny, speed, TEX::MagicOrb);
		g._entity_handler._entities_to_add.push_back(new_shot);

	}

	// check if distance to player is too far away => go closer
	float distance_to_player_squared;
	{
		float dx = mid_x() - g._entity_handler._p.mid_x();
		float dy = mid_y() - g._entity_handler._p.mid_y();
		distance_to_player_squared = dx * dx + dy * dy;
	}
	if (distance_to_player_squared > _get_closer_distance_squared)
	{
		// too far away
		if (g._tile_handler.is_path_clear(g, x(), y(), g._entity_handler._p.x(), g._entity_handler._p.y()))
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
				if (x() > tile_x && y() > tile_y &&
					x() + w() < tile_x + g._cam._grid && y() + h() < tile_y + g._cam._grid)
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