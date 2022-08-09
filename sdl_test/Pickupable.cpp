#include "Pickupable.h"
#include "Game.h"

MOVING_RECT_TYPES Pickupable::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::PICKUPABLE;
}

Pickupable::Pickupable(PICKUPABLE_TYPE type, float x, float y, float x_vel, float y_vel)
	: _type(type), MovingRect(0, 0, 30.f, 30.f, 0.003f)
{

	if (_type == PICKUPABLE_TYPE::COIN) {
		_texture = TEX::Coin;
	}
	else if (_type == PICKUPABLE_TYPE::SHOT) {
		_texture = TEX::Container;
	}
	else if (_type == PICKUPABLE_TYPE::BOMB) {

	}
	else if (_type == PICKUPABLE_TYPE::HEART) {

	}

	set_x(x - get_half_w());
	set_y(y - get_half_h());

	change_x_vel(x_vel);
	change_y_vel(y_vel);
}

bool Pickupable::logic(Game& g)
{
	this->move_and_collide<true>(g);

	// death == death
	if (_picked_up)
	{
		delete this;
		return true;
	}
	return false;
}

void Pickupable::draw(Game& g)
{
	SDL_Rect rect = { g._cam.convert_x((int)_x), g._cam.convert_y((int)_y),(int)_w,(int)_h };
	SDL_RenderCopy(g._renderer, g._textures[_texture], NULL, &rect);
}

void Pickupable::intersection(Game& g, float nx, float ny, MovingRect* e)
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
		_picked_up = true;
		break;
	}
	case MOVING_RECT_TYPES::PICKUPABLE:
	{
		float bounce_acc = 0.05f;

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);
		break;
	}
	}
}
