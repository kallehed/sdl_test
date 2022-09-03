#include "Chest.h"
#include "Game.h"

MOVING_RECT_TYPES Chest::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::CHEST;
}

Chest::Chest(int onetime_index, float x, float y, int amount, CHEST_TYPE type)
	: MovingRect(x, y, 64, 58, 1.f),
	_onetime_index(onetime_index),
	_chest_amount(amount),
	_type(type)
{
	using enum CHEST_TYPE;
	switch (_type) {
	case COIN_CHEST:
		_tex = TEX::Chest;
		_text1 = "Chest contained ";
		_text2 = " coins!\nLucky you.";
		break;
	case HP_UPGRADE:
		_tex = TEX::HeartContainer;
		set_w(15.f * 3.f);
		set_h(26.f * 3.f);
		_text1 = "Max HP increased by ";
		_text2 = " HP!\nHealthy!";
		break;
	}
}

bool Chest::logic(Game& g)
{
	Player& p = g._entity_handler._p;

	if (!_showing_text) {
		constexpr float threshold = 85.f;
		_show_e_sign = threshold > abs(mid_x() - p.mid_x()) + abs(mid_y() - p.mid_y());

		if (_show_e_sign) {
			if (g._keys_frame[SDL_SCANCODE_E])
			{
				// Chest actived, destroy + give item(s)

				switch (_type) {
				case CHEST_TYPE::COIN_CHEST:
					// GIVE PLAYER MONEYYYYYYYYY
					p._coins += _chest_amount;
					break;
				case CHEST_TYPE::HP_UPGRADE:
					p._max_hp += _chest_amount;
					p._hp += _chest_amount;
					break;
				}

				g._save._onetimes.insert({ g._level, _onetime_index });

				_showing_text = true;
				_show_e_sign = true;
				return false;
			}
		}
	}
	else {
		// showing text
		constexpr float threshold = 150.f;
		bool far_away = threshold < abs(mid_x() - p.mid_x()) + abs(mid_y() - p.mid_y());

		if (far_away || g._keys_frame[SDL_SCANCODE_E]) {
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

		SDL_RenderCopy(g._renderer, g._textures[_tex], NULL, &rect);
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
			std::string text = _text1 + std::to_string(_chest_amount) + _text2;
			g._cam.draw_text(g, text.c_str(), {0,0,0,255}, x + text_offset, y + text_offset, 3);
		}
	}
}

void Chest::intersection(Game& g, float nx, float ny, MovingRect* e)
{
}
