#include "FireMagic.h"
#include "Game.h"

MOVING_RECT_TYPES FireMagic::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::FIRE_MAGIC;
}

FireMagic::FireMagic(MovingRect* owner, float x, float y)
	: MovingRect(0, 0, 50.f, 50.f, 0.0f), _owner(owner)
{
	set_x(x - get_half_w());
	set_y(y - get_half_h());
}

// first frame, be in _entities, then next, remove self, add to _draw_entities
// then, wait out a bit, then DELETE SELF, and from _draw_entities
bool FireMagic::logic(Game& g)
{
	if (_exploded == false)
	{
		_exploded = true;

		// possibly kill destructible tiles
		{
			int j_start = g._cam.convert_x_to_j(_x);
			int j_end = g._cam.convert_x_to_j((_x + _w - 0.01f)) + 1;
			int i_start = g._cam.convert_y_to_i(_y);
			int i_end = g._cam.convert_y_to_i((_y + _h - 0.01f)) + 1;

			for (int i = i_start; i < i_end; ++i)
			{
				for (int j = j_start; j < j_end; ++j)
				{
					if (g._tile_handler.hurt_tile(g, i, j))
					{
						return false; // only kill one destructable
					}
				}
			}
		}
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

void FireMagic::draw(Game& g)
{
	int alpha_val = (int)(300.f * (1.f - (_explosion_timer / _explosion_time)));
	const SDL_Rect dst = { g._cam.convert_x((int)_x), g._cam.convert_y((int)_y),(int)_w,(int)_h };
	/*
	SDL_SetRenderDrawColor(g._renderer, 255, 50, 50, alpha_val);

	SDL_Rect rect = { g._cam.convert_x((int)get_x()), g._cam.convert_y((int)get_y()),(int)get_w(),(int)get_h() };
	SDL_RenderFillRect(g._renderer, &rect);
	*/
	SDL_SetTextureAlphaMod(g._textures[TEX::FireMagic], (unsigned char)alpha_val);
	SDL_RenderCopy(g._renderer, g._textures[TEX::FireMagic], NULL, &dst);
}

void FireMagic::intersection(float nx, float ny, MovingRect* e)
{
}
