#include "Bonfire.h"
#include "Game.h"

MOVING_RECT_TYPES Bonfire::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::BONFIRE;
}

Bonfire::Bonfire(float x, float y)
	: MovingRect(x, y, 64, 81, 1.f)
{
}

bool Bonfire::logic(Game& g)
{
	Player& p = g._entity_handler._p;

	if (_fire_animation) {
		_animation_timer += g._dt;

		if (_animation_timer > _animation_time) {
			_fire_animation = false;
			_animation_timer = 0.f;

			// HEAL PLAYER
			p._hp = p._max_hp; 
			// SET RESPAWN POINT
			p._respawn_x = _x;
			p._respawn_y = _y;
			p._respawn_level = g._level;
		}
	}
	else {

		constexpr float threshold = 85.f;
		_player_close = threshold > abs(mid_x() - p.mid_x()) + abs(mid_y() - p.mid_y());

		if (_player_close) {
			if (g._keys_frame[SDL_SCANCODE_E])
			{
				// bonfire player
				if (!_fire_animation) {
					// will be healed at end of animation
					_fire_animation = true;
					_player_close = false;
					g.play_sound(SOUND::Bonfire);
				}
			}
		}
	}

	return false;
}

void Bonfire::draw(Game& g)
{
	// Sprite and "Press E" box
	{
		const SDL_Rect rect = { g._cam.convert_x((int)_x), g._cam.convert_y((int)_y),(int)_w,(int)_h };

		//TEX::TEX tex = TEX::Bonfire;

		SDL_RenderCopyEx(g._renderer, g._textures[TEX::Bonfire], NULL, &rect, 0.0, NULL, (SDL_RendererFlip)((g._ticks/35) % 2));

		// "Press E" Text
		if (_player_close) {
			constexpr int scale = 3;
			const SDL_Rect text_rect = { rect.x - 25 + (int)(10.f * sinf(((float)g._ticks) * 0.1f)), rect.y - 50, g._press_e_w_and_h[0] * scale, g._press_e_w_and_h[1] * scale };
			SDL_SetRenderDrawColor(g._renderer, 255, 255, 255, 127);
			SDL_RenderFillRect(g._renderer, &text_rect);
			SDL_RenderCopy(g._renderer, g._press_e_texture, NULL, &text_rect);
		}

		// draw weird color gradient thing that possibly looks cool sometimes
		if (_fire_animation)
		{ 
			uint8_t alpha = (Uint8)(255.f * (_animation_timer / _animation_time));

			constexpr float vertices[4*2] =
			{ 0,0,
			g._WIDTH, 0,
			0, g._HEIGHT,
			g._WIDTH, g._HEIGHT };

			uint8_t c_val = g._ticks % 256;
			uint8_t c_val2 = (g._ticks*2 + 126) % 256;
			uint8_t c_val3 = (g._ticks * 3) % 256;
			SDL_Color c1 = { c_val,(Uint8)(c_val3%100Ui8),0,alpha };
			SDL_Color c2 = { c_val3,0,0,alpha };
			SDL_Color c3 = { c_val2,(Uint8)(c_val%100Ui8),0,alpha };
			SDL_Color colors[4] = { c1,c2, c2,c3 };

			constexpr uint8_t indices[6] =
			{ 0, 1, 2,
			1, 2, 3 };

			SDL_RenderGeometryRaw(g._renderer, NULL,
				vertices, 2*sizeof(float), colors, sizeof(SDL_Color), NULL, 0, 4,
				indices, 6, sizeof(uint8_t));
		}
	}
}

void Bonfire::intersection(Game& g, float nx, float ny, MovingRect* e)
{
}
