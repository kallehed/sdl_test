#include "Particle.h"
#include "Game.h"

MOVING_RECT_TYPES Particle::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::PARTICLE;
}

Particle::Particle(float x, float y, float x_vel, float y_vel, SDL_Color color)
	: MovingRect(0, 0, (float)(3 + rand()%6), (float)(3 + rand()%6), 0.003f),
	_color(color)
{
	set_x(x - get_half_w());
	set_y(y - get_half_h());

	//change_x_vel(x_vel / 80.f);
	//change_y_vel(y_vel / 80.f);
}

bool Particle::logic(Game& g)
{
	this->move_and_collide<true>(g);

	// death == death
	_timer -= g._dt;
	if (_timer <= 0.f)
	{
		delete this;
		return true;
	}
	return false;
}

void Particle::draw(Game& g)
{
	SDL_SetRenderDrawColor(g._renderer, _color.r, _color.g, _color.b, _color.a);

	SDL_Rect rect = { g._cam.convert_x((int)get_x()), g._cam.convert_y((int)get_y()),(int)get_w(),(int)get_h() };
	SDL_RenderFillRect(g._renderer, &rect);
	
}

void Particle::intersection(Game& g, float nx, float ny, MovingRect* e)
{
}