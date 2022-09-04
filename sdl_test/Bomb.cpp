#include "Bomb.h"
#include "Game.h"
#include "Explosion.h"

MOVING_RECT_TYPES Bomb::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::BOMB;
}

Bomb::Bomb(float x, float y, float x_vel, float y_vel, int damage, float area_factor, float detonation_time)
	: MovingRect(0, 0, 30.f, 30.f, 0.003f),
	_damage(damage),
	_area_factor(area_factor),
	_detonation_timer(detonation_time)
{
	set_x(x - half_w());
	set_y(y - half_h());

	change_x_vel(x_vel / 20.f);
	change_y_vel(y_vel / 20.f);
}

bool Bomb::logic(Game& g)
{
	this->move_and_collide<true>(g);

	// death == death
	_detonation_timer -= g._dt;
	if (_detonation_timer <= 0.f || _lives < 1) // detonate
	{
		g._entity_handler._entities_to_add.push_back(new Explosion(mid_x(), mid_y(), _damage, _area_factor));
		
		// shake screen
		g._cam.shake(g, 1.20f, 50.f);

		delete this;
		return true;
	}
	return false;
}

void Bomb::draw(Game& g)
{
	SDL_Texture* tex = NULL;

	if (((((int)_detonation_timer) / 500) % 2) == 0) {
		//SDL_SetRenderDrawColor(g._renderer, 175, 0, 0, 255);
		tex = g._textures[TEX::Bomb];
	}
	else {
		//SDL_SetRenderDrawColor(g._renderer, 50, 0, 0, 255);
		tex = g._textures[TEX::BombRed];
	}

	//SDL_SetRenderDrawColor(g._renderer, 175, 0, 0, 255);

	SDL_Rect rect = { g._cam.convert_x((int)x()), g._cam.convert_y((int)y()),(int)w(),(int)h() };
	//SDL_RenderFillRect(g._renderer, &rect);
	SDL_RenderCopy(g._renderer, tex, NULL, &rect);
}

void Bomb::intersection(Game& g, float nx, float ny, MovingRect* e)
{
	switch (e->get_moving_rect_type()) {
	case MOVING_RECT_TYPES::ENEMY:
	{
		float bounce_acc = 0.001f;

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);
		break;
	}
	case MOVING_RECT_TYPES::PLAYER:
	{
		float bounce_acc = 0.001f; // move little bit

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);
		break;
	}
	case MOVING_RECT_TYPES::BOMB:
	{
		float bounce_acc = 0.02f;

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);
		break;
	}
	case MOVING_RECT_TYPES::SHOT:
	{
		_lives -= 1;
		_detonation_timer /= 2.f; // halve time til explosion

		float bounce_acc = 0.04f;

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);
		break;
	}
	case MOVING_RECT_TYPES::EXPLOSION:
	{
		_lives = 0; // detonate IMMEDIATELY(soon)
		break;
	}
	}
}
