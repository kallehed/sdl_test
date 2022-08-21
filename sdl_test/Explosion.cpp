#include "Explosion.h"
#include "Game.h"

MOVING_RECT_TYPES Explosion::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::EXPLOSION;
}

Explosion::Explosion(float x, float y, int damage, float area_factor)
	: MovingRect(0, 0, area_factor*300.f, area_factor*300.f, 0.0f),
	_damage(damage)
{
	set_x(x - half_w());
	set_y(y - half_h());
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

	int alpha_val = (int)(255.f * (1.f - (_explosion_timer / _explosion_time)));
	const SDL_Rect dst = { g._cam.convert_x((int)_x), g._cam.convert_y((int)_y),(int)_w,(int)_h };

	SDL_SetTextureAlphaMod(g._textures[TEX::BombExplosion], (unsigned char)alpha_val);
	SDL_RenderCopy(g._renderer, g._textures[TEX::BombExplosion], NULL, &dst);
}

void Explosion::intersection(Game& g, float nx, float ny, MovingRect* e)
{
}
