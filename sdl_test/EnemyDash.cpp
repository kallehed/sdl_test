#include "EnemyDash.h"
#include "Game.h"
#include "Shot.h"
#include "Explosion.h"

MOVING_RECT_TYPES EnemyDash::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::ENEMY;
}

EnemyDash::EnemyDash(float x, float y)
	: Enemy(x, y, 38.f, 50.f, 20, 200.f, 300.f, 7000.f,
		0.0018f, 2000.f, 1500.f, 200.f - 50 * General::randf01(), 200.f - 50 * General::randf01(), 0.0018f)
{
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

void EnemyDash::active_logic(Game& g)
{	
	auto& p = g._entity_handler._p;

	switch (_state) {
	case ENEMY_DASH_STATE::WAITING:
	{
		if (_active_timer > 0.f)
		{
			_active_timer -= g._dt;

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
