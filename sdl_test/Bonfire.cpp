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
		}
	}
	else {

		constexpr float threshold = 85.f;
		_player_close = threshold > abs(get_mid_x() - p.get_mid_x()) + abs(get_mid_y() - p.get_mid_y());

		if (_player_close) {
			if (g._keys_frame[SDLK_e])
			{
				// bonfire player
				if (!_fire_animation) {
					// will be healed at end of animation
					_fire_animation = true;
					_player_close = false;
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

		SDL_RenderCopy(g._renderer, g._textures[TEX::Bonfire], NULL, &rect);

		// "Press E" Text
		if (_player_close) {
			constexpr int scale = 3;
			const SDL_Rect text_rect = { rect.x - 25 + (int)(10.f * sinf(((float)g._ticks) * 0.1f)), rect.y - 50, g._press_e_w_and_h[0] * scale, g._press_e_w_and_h[1] * scale };
			SDL_SetRenderDrawColor(g._renderer, 255, 255, 255, 127);
			SDL_RenderFillRect(g._renderer, &text_rect);
			SDL_RenderCopy(g._renderer, g._press_e_texture, NULL, &text_rect);
		}

		if (_fire_animation) {
			const SDL_Rect a_rect = { 0, 0, g._WIDTH, g._HEIGHT };
			SDL_SetRenderDrawColor(g._renderer, 255, 50, 50, (Uint8)(255.f * (_animation_timer / _animation_time)));
			SDL_RenderFillRect(g._renderer, &a_rect);
		}
	}
}

void Bonfire::intersection(Game& g, float nx, float ny, MovingRect* e)
{
}
