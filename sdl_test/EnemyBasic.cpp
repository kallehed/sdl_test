#include "EnemyBasic.h"
#include "Game.h"
#include <math.h>
#include "Explosion.h"
#include "FireMagic.h"
#include "Particle.h"

MOVING_RECT_TYPES EnemyBasic::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::ENEMY;
}

// TODO: Make Destructible tile only give hearts + ammo.
// + There should only be one "Pickupable" class.
// + Enemies should be able to give variable coin amounts
// + Coins should be displayed by number instead of bar
// 

EnemyBasic::EnemyBasic(float x, float y) : Enemy(x, y, 40.f,40.f, 20, 150.f, 200.f, 5000.f)
{
}

void EnemyBasic::draw(Game& g)
{
	SDL_SetRenderDrawColor(g._renderer, 0, 0, 255, 255);

	SDL_Rect rect = { g._cam.convert_x((int)x()), g._cam.convert_y((int)y()),(int)w(),(int)h() };
	//SDL_RenderFillRect(g._renderer, &rect);

	draw_circle(g._renderer, rect.x + _w / 2, rect.y + _h * 0.9f, 20, { 0,0,0,67 });

	// 0 is nothing, 1 is flip horizontally
	SDL_RendererFlip flip = (x_vel() > 0.f) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

	if (_hurt_timer > 0.f) {
		SDL_SetTextureColorMod(g._textures[TEX::BlueSlime], 100, 100, 100);
		_hurt_timer -= g._dt;
	}
	else if (_active_increased_speed) { // make it read
		//SDL_SetTextureColorMod(g._textures[TEX::BlueSlime], 255, 200, 200);
		SDL_SetTextureColorMod(g._textures[TEX::BlueSlime], 255, 255, 255);
		SDL_SetRenderDrawBlendMode(g._renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(g._renderer, 255, 0, 0, 255);
		SDL_RenderFillRect(g._renderer, &rect);
		SDL_SetTextureAlphaMod(g._textures[TEX::BlueSlime], 126);
		SDL_RenderCopyEx(g._renderer, g._textures[TEX::BlueSlime], NULL, &rect, NULL, NULL, flip);
		SDL_SetTextureAlphaMod(g._textures[TEX::BlueSlime], 255);
		return;
	}
	else {
		SDL_SetTextureColorMod(g._textures[TEX::BlueSlime], 255, 255, 255);
	}

	SDL_RenderCopyEx(g._renderer, g._textures[TEX::BlueSlime], NULL, &rect, NULL, NULL, flip);
}

void EnemyBasic::idle_logic(Game& g)
{
	_timer += g._dt;
	if (!_idle_state) // wait for a moment still
	{
		if (_timer > 3000.f) {
			_idle_state = true;
			_timer = 1000.f * General::randf01();

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

void EnemyBasic::take_damage(Game& g, int damage)
{
	_hp -= damage;
	_hurt_timer = 150.f;

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

