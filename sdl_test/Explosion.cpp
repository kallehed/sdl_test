#include "Explosion.h"
#include "Game.h"

MOVING_RECT_TYPES Explosion::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::EXPLOSION;
}

Explosion::Explosion(float x, float y) : MovingRect(0, 0, 200.f, 200.f, 0.0f)
{
	set_x(x - get_half_w());
	set_y(y - get_half_h());
}

// first frame, be in _entities, then next, remove self, add to _draw_entities
// then, wait out a bit, then DELETE SELF, and from _draw_entities
bool Explosion::logic(Game& g)
{
	if (_exploded == false)
	{
		_exploded = true;
	}
	else
	{
		if (_explosion_timer == 0.f) {
			// remove self from _entities, add to _draw_entitites
			_explosion_timer += g._dt;
			g._entity_handler._draw_entities.push_back(this);
			return true;
		}
		_explosion_timer += g._dt;
		if (_explosion_timer >= 2500.f)
		{
			delete this;
			return true;
		}
	}
	return false;
}

void Explosion::draw(Game& g)
{
	SDL_SetRenderDrawColor(g._renderer, 255, 50, 50, 255);

	SDL_Rect rect = { g._cam.convert_x((int)get_x()), g._cam.convert_y((int)get_y()),(int)get_w(),(int)get_h() };
	SDL_RenderFillRect(g._renderer, &rect);
}

void Explosion::intersection(float nx, float ny, MovingRect* e)
{
	/*
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
	case MOVING_RECT_TYPES::EXPLOSION:
	{
		float bounce_acc = 0.05f;

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);
		break;
	}
	case MOVING_RECT_TYPES::SHOT_ENEMY:
	case MOVING_RECT_TYPES::SHOT:
	{
		_lives -= 1;

		float bounce_acc = 0.04f;

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);
		break;
	}
	}
	*/
}
