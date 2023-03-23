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

EnemyBasic::EnemyBasic(float x, float y) : Enemy(x, y, 40.f,40.f, 20, 150.f, 200.f, 5000.f, 0.0015f, 3000.f,2000.f, 0.f, 0.f, 0.001f)
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
	else if (_state == ENEMY_BASIC_STATE::ANGRY) { // make it RED
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

void EnemyBasic::take_damage(Game& g, int damage)
{
	// call Enemy's take_damage
	this->Enemy::take_damage(g, damage);

	// possibly get scared
	int randint = rand() % 100;
	if (randint >= 80) // 20 % chance
	{
		if (randint >= 90) {
			_state = ENEMY_BASIC_STATE::ANGRY;
			_active_timer = 0.f;
		}
		else {
			_state = ENEMY_BASIC_STATE::AFRAID;
			_active_timer = 0.f;
		}	
	}
}

void EnemyBasic::active_logic(Game& g)
{
	using namespace ENEMY_BASIC_STATE;
	switch (_state) {
	case NORMAL: // go to player
	{
		stay_in_range_of_player(g);
		break;
	}
	case AFRAID: // run away
	{
		go_towards_player(g, (-1.f) * _active_basic_speed);
		_active_timer += g._dt;
		if (_active_timer > 3000) {
			_state = NORMAL;
		}
		break;
	}
	case ANGRY: // run to player IN RED
	{
		go_towards_player(g, _active_basic_speed * 2.f);
		_active_timer += g._dt;
		if (_active_timer > 3000) {
			_state = NORMAL;
		}
		break;
	}
	}
}

