#include "PickupableShot.h"

#include "Game.h"

MOVING_RECT_TYPES PickupableShot::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::PICKUPABLE_SHOT;
}

PickupableShot::PickupableShot(float x, float y, float x_vel, float y_vel) : MovingRect(0, 0, 30.f, 30.f, 0.003f)
{
	set_x(x - get_half_w());
	set_y(y - get_half_h());

	change_x_vel(x_vel);
	change_y_vel(y_vel);
}

bool PickupableShot::logic(Game& g)
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

void PickupableShot::draw(Game& g)
{
	SDL_SetRenderDrawColor(g._renderer, 98, 12, 93, 255);

	SDL_Rect rect = { g._cam.convert_x((int)_x), g._cam.convert_y((int)_y),(int)_w,(int)_h };
	//SDL_RenderFillRect(g._renderer, &rect);
	SDL_RenderCopy(g._renderer, g._textures[TEX::Container], NULL, &rect);
}

void PickupableShot::intersection(Game& g, float nx, float ny, MovingRect* e)
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
