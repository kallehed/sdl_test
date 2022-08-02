#include "Explosion.h"
#include "Game.h"

MOVING_RECT_TYPES Explosion::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::EXPLOSION;
}

Explosion::Explosion(float x, float y) : MovingRect(0, 0, 300.f, 300.f, 0.0f)
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
		if (_explosion_timer >= _explosion_time)
		{
			delete this;
			return true;
		}
	}
	return false;
}

void Explosion::draw(Game& g)
{
	/*int alpha_val = (int)(255.f * (1.f - (_explosion_timer / _explosion_time)));

	SDL_SetRenderDrawColor(g._renderer, 255, 50, 50, alpha_val);

	SDL_Rect rect = { g._cam.convert_x((int)get_x()), g._cam.convert_y((int)get_y()),(int)get_w(),(int)get_h() };
	SDL_RenderFillRect(g._renderer, &rect);
	*/

	int alpha_val = (int)(255.f * (1.f - (_explosion_timer / _explosion_time)));
	const SDL_Rect dst = { g._cam.convert_x((int)_x), g._cam.convert_y((int)_y),(int)_w,(int)_h };

	SDL_SetTextureAlphaMod(g._textures[TEX::BombExplosion], (unsigned char)alpha_val);
	SDL_RenderCopy(g._renderer, g._textures[TEX::BombExplosion], NULL, &dst);
}

void Explosion::intersection(float nx, float ny, MovingRect* e)
{
}
