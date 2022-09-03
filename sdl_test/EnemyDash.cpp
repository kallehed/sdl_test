#include "EnemyDash.h"
#include "Game.h"
#include "Shot.h"
#include "Explosion.h"

MOVING_RECT_TYPES EnemyDash::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::ENEMY;
}

EnemyDash::EnemyDash(float x, float y) : Enemy(x, y, 38.f, 50.f, 20, 200.f, 300.f, 7000.f)
{
	_get_away_distance_squared = powf(200.f - 50 * General::randf01(), 2);
	_get_closer_distance_squared = powf(200.f - 50 * General::randf01(), 2);
}

void EnemyDash::draw(Game& g)
{
	SDL_Rect rect = { g._cam.convert_x((int)x()), g._cam.convert_y((int)y()),(int)(_w*1.2f),(int)(_h*1.2f) };

	draw_circle(g._renderer, rect.x + _w/2.f, rect.y + rect.h * 0.9f, 15, { 0,0,0,67 });

	SDL_RendererFlip flip = (x_vel() > 0.f) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

	TEX::TEX tex =  TEX::Cactus;

	if (_state == ENEMY_DASH_STATE::DASHING) {
		tex = TEX::CactusAngry;

		flip = (SDL_RendererFlip)((g._ticks / 7) % 2);
	}

	if (_hurt_timer > 0.f) {
		SDL_SetTextureColorMod(g._textures[tex], 100, 100, 100);
		_hurt_timer -= g._dt;
	}
	else {
		SDL_SetTextureColorMod(g._textures[tex], 255, 255, 255);
	}

	SDL_RenderCopyEx(g._renderer, g._textures[tex], NULL, &rect, NULL, NULL, flip);
}

void EnemyDash::idle_logic(Game& g)
{
	_timer += g._dt;
	if (!_idle_state) // wait for a moment still
	{
		if (_timer > 2000.f) {
			_idle_state = true;
			_timer = 0.f;

			float random_radians = (General::pi() * (rand() % 360)) / 180.f;
			_idle_x_dir = cos(random_radians);
			_idle_y_dir = sin(random_radians);
		}
	}
	else { // walk
		change_x_vel(_idle_speed * _idle_x_dir);
		change_y_vel(_idle_speed * _idle_y_dir);
		if (_timer > 1500.f) {
			_idle_state = false;
			_timer = 0.f;
		}
	}
}

void EnemyDash::take_damage(Game& g, int damage) {
	_hp -= damage;
	_hurt_timer = 150.f;
}

void EnemyDash::active_logic(Game& g)
{	
	auto& p = g._entity_handler._p;

	switch (_state) {
	case ENEMY_DASH_STATE::WAITING:
	{
		if (_active_timer > 0.f)
		{
			_active_timer -= g._dt;

			// stay in range from player
			stay_in_range_of_player(g);
		}
		else
		{
			_active_timer = 0;
			_state = ENEMY_DASH_STATE::CHARGING;
		}
		break;
	}
	case ENEMY_DASH_STATE::CHARGING:
	{

		const auto [nx, ny, _] = General::normalize_vector_two_points_arr(p.mid_x(), p.mid_y(), mid_x(), mid_y());

		change_x_vel(nx * 0.0006f);
		change_y_vel(ny * 0.0006f);

		_active_timer += g._dt;
		if (_active_timer >= 1000.f) {
			_active_timer = 0.f;
			_state = ENEMY_DASH_STATE::DASHING;
			_dash_x = -nx * 0.005f;
			_dash_y = -ny * 0.005f;
		}

		break;
	}
	case ENEMY_DASH_STATE::DASHING:
	{
		change_x_vel(_dash_x);
		change_y_vel(_dash_y);

		_active_timer += g._dt;
		if (_active_timer >= 1500.f) {
			_state = ENEMY_DASH_STATE::WAITING;
			_active_timer = 1500.f + 1000.f * General::randf01();
		}
		break;
	}
	}
}

void EnemyDash::stay_in_range_of_player(Game& g)
{
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
