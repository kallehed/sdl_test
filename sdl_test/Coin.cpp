#include "Coin.h"
#include "Game.h"

MOVING_RECT_TYPES Coin::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::COIN;
}

Coin::Coin(float x, float y, float x_vel, float y_vel) : MovingRect(0, 0, 30.f, 30.f, 0.003f)
{
	set_x(x - get_half_w());
	set_y(y - get_half_h());

	/*float speed = 0.04f;
	float degree = General::randf01() * 2.f * (float)M_PI;

	change_x_vel(cosf(degree) * speed);
	change_y_vel(sinf(degree) * speed);*/

	change_x_vel(x_vel);
	change_y_vel(y_vel);
}

bool Coin::logic(Game& g)
{
	this->move_and_collide<true>(g);

	// death == death
	if (_lives < 1)
	{
		delete this;
		return true;
	}
	return false;
}

void Coin::draw(Game& g)
{
	SDL_SetRenderDrawColor(g._renderer, 255, 255, 0, 255);

	SDL_Rect rect = { g._cam.convert_x((int)get_x()), g._cam.convert_y((int)get_y()),(int)get_w(),(int)get_h() };
	SDL_RenderFillRect(g._renderer, &rect);
}

void Coin::intersection(float nx, float ny, MovingRect* e)
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
		_lives -= 1;
		break;
	}
	case MOVING_RECT_TYPES::COIN:
	{
		float bounce_acc = 0.05f;

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);
		break;
	}
	}
}
