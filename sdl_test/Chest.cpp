#include "Chest.h"
#include "Game.h"

MOVING_RECT_TYPES Chest::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::CHEST;
}

Chest::Chest(int onetime_index, float x, float y)
	: MovingRect(x, y, 60, 60, 1.f),
	_onetime_index(onetime_index)
{
}

bool Chest::logic(Game& g)
{
	Player& p = g._entity_handler._p;

	if (!_showing_text) {
		constexpr float threshold = 85.f;
		_show_e_sign = threshold > abs(get_mid_x() - p.get_mid_x()) + abs(get_mid_y() - p.get_mid_y());

		if (_show_e_sign) {
			if (g._keys_frame[SDLK_e])
			{
				// GIVE PLAYER MONEYYYYYYYYY
				g._entity_handler._p._coins += _chest_amount;
				g._onetimes.insert({ g._level, _onetime_index });
				//g._onetime_indexes[g._level * g._INDEX_PER_LEVEL + _onetime_index] = true;

				_showing_text = true;
				_show_e_sign = true;
				return false;
			}
		}
	}
	else {
		// showing text
		constexpr float threshold = 150.f;
		bool far_away = threshold < abs(get_mid_x() - p.get_mid_x()) + abs(get_mid_y() - p.get_mid_y());

		if (far_away || g._keys_frame[SDLK_e]) {
			delete this;
			return true;
		}
	}

	return false;
}
void Chest::draw(Game& g)
{
	// Sprite and "Press E" box
	{
		const SDL_Rect rect = { g._cam.convert_x((int)_x), g._cam.convert_y((int)_y),(int)_w,(int)_h };

		SDL_RenderCopy(g._renderer, g._textures[TEX::Chest], NULL, &rect);
		// "Press E" Text
		if (_show_e_sign) {
			constexpr int scale = 3;
			const SDL_Rect text_rect = { rect.x - 25 + (int)(10.f * sinf(((float)g._ticks) * 0.1f)), rect.y - 50, g._press_e_w_and_h[0] * scale, g._press_e_w_and_h[1] * scale };
			SDL_SetRenderDrawColor(g._renderer, 255, 255, 255, 127);
			SDL_RenderFillRect(g._renderer, &text_rect);
			SDL_RenderCopy(g._renderer, g._press_e_texture, NULL, &text_rect);
		}

		// Talking box
		if (_showing_text) {
			SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, 255);
			int y = (int)((float)g._HEIGHT * 0.575f);
			int x = 267;
			int scale = 20;
			const SDL_Rect box = { x, y, 32 * scale, 15 * scale };
			SDL_RenderCopy(g._renderer, g._textures[TEX::DialogueBox], NULL, &box);

			constexpr int text_offset = 40;
			std::string text = "Chest contained " + std::to_string(_chest_amount) + " coins!\nLucky you.";
			g._cam.draw_text(g, text.c_str(), {0,0,0,255}, x + text_offset, y + text_offset, 3);
		}
	}
}

void Chest::intersection(Game& g, float nx, float ny, MovingRect* e)
{
}
