#include "EnemyJump.h"
#include "Game.h"
#include "Shot.h"
#include "Explosion.h"
#include "Bomb.h"

MOVING_RECT_TYPES EnemyJump::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::ENEMY;
}

EnemyJump::EnemyJump(float x, float y)
	: Enemy(x, y, 37.f, 32.f, 20, 300.f, 400.f, 7000.f,
		0.0018f, 2000.f, 1500.f, 200.f - 50 * General::randf01(), 200.f - 50 * General::randf01(), 0.0018f)
{
}

void EnemyJump::draw(Game& g)
{
	SDL_Rect draw_rect = { g._cam.convert_x((int)x()), g._cam.convert_y((int)y()) - 32,(int)(37*1.2),(int)(64*1.2) };
	draw_circle(g._renderer, draw_rect.x + _w / 2.f, draw_rect.y + draw_rect.h * 0.9f, 15, { 0,0,0,67 });

	SDL_RendererFlip flip = (x_vel() > 0.f) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

	TEX::TEX tex = TEX::BombMan;

	if (_state != ENEMY_JUMP_STATE::WAITING)
	{
		if ((g._ticks / 7)%2 == 0)
		{
			tex = TEX::BombMan2;
		}
		
	}

	if (_hurt_timer > 0.f) {
		SDL_SetTextureColorMod(g._textures[tex], 100, 100, 100);
		_hurt_timer -= g._dt;
	}
	else {
		SDL_SetTextureColorMod(g._textures[tex], 255, 255, 255);
	}

	draw_rect.y += (int)_jump_y_dif;
	SDL_RenderCopyEx(g._renderer, g._textures[tex], NULL, &draw_rect, NULL, NULL, flip);
}

void EnemyJump::active_logic(Game& g)
{
	auto& p = g._entity_handler._p;
	
	switch (_state) {
	case ENEMY_JUMP_STATE::WAITING:
	{
		if (_active_timer > 0.f)
		{
			_active_timer -= g._dt;

			stay_in_range_of_player(g);
		}
		else
		{
			const auto [nx, ny, distance] = General::normalize_vector_two_points_arr(mid_x(), mid_y(), p.mid_x(), p.mid_y());

			if (General::randf01() < 0.65f) // jump
			{
				_state = ENEMY_JUMP_STATE::JUMPING;
				float speed = 0.005f + 0.002f * (General::randf01() - 0.4f);
				_jump_x = nx * speed;
				_jump_y = ny * speed;
				_jump_vel = -7.f - 2.f * (General::randf01() - 0.4f);
			}
			else // dash
			{
				_state = ENEMY_JUMP_STATE::DASHING;
				float speed = 0.005f + 0.002f * (General::randf01() - 0.4f);
				_jump_x = nx * speed;
				_jump_y = ny * speed;
				_active_timer = 1000.f;
			}
		}
		break;
	}
	case ENEMY_JUMP_STATE::JUMPING:
	{
		change_x_vel(_jump_x);
		change_y_vel(_jump_y);

		_jump_y_dif += _jump_vel;
		_jump_vel += 0.0125f * g._dt;

		if ( !_bombed &&_jump_vel > 0.f) {
			_bombed = true;
			
			g._entity_handler._entities_to_add.push_back(new Bomb(g, mid_x(), mid_y(), 0.f, 0.f, 7, 1.f, 400.f));
		}

		if (_jump_y_dif > 0.f) {
			_state = ENEMY_JUMP_STATE::WAITING;
			_jump_y_dif = 0.f;
			_active_timer = 1500.f + 1000.f * General::randf01();
			_bombed = false;
		}
		break;
	}
	case ENEMY_JUMP_STATE::DASHING:
	{
		change_x_vel(_jump_x);
		change_y_vel(_jump_y);

		_active_timer -= g._dt;
		if (_active_timer <= 0.f) {
			_active_timer = 750.f + 500.f * General::randf01();
			_state = ENEMY_JUMP_STATE::WAITING;
		}

		break;
	}
	}
}

void EnemyJump::insert_to_intersect(EntityHandler& entitiy_handler)
{
	if (_state != ENEMY_JUMP_STATE::JUMPING) {
		entitiy_handler._quadtree.add_to_head(this);
	}
}
