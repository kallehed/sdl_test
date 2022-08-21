#include "Particle.h"
#include "Game.h"

MOVING_RECT_TYPES Particle::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::PARTICLE;
}

Particle::Particle(float x, float y, float x_vel, float y_vel, SDL_Color color, int thick)
	: MovingRect(0, 0, (float)(3 + rand()%thick), (float)(3 + rand()%thick), 0.003f)
{
	set_x(x - half_w());
	set_y(y - half_h());

	change_x_vel(x_vel / 40.f);
	change_y_vel(y_vel / 40.f);
	_timer = 10000.f + rand()%20000; // LOOONG, to be able to witness the spectacle afterwards

	// change color
	float t = (0.75f + 0.25f * General::randf01());

	_color = { (Uint8)((float)color.r * t),(Uint8)((float)color.g * t),(Uint8)((float)color.b * t),255 }; // MAX OPAQUENESS FOR COOLNESS'S SAKE
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

	SDL_Rect rect = { g._cam.convert_x((int)x()), g._cam.convert_y((int)y()),(int)w(),(int)h() };
	SDL_RenderFillRect(g._renderer, &rect);
	
}

void Particle::intersection(Game& g, float nx, float ny, MovingRect* e)
{
}
