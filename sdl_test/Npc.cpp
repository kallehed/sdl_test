#include "Npc.h"
#include "Game.h"
#include "General.h"

MOVING_RECT_TYPES Npc::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::NPC;
}

Npc::Npc(Game& g, NPC_TYPE type, float x, float y)
	: MovingRect(x, y, 70, 80, 1.f), _npc_type(type)
{
	switch (_npc_type) {
	case NPC_TYPE::NPC1:
		_text = g._entity_handler._NPC_1_TEXT;
		_total_chars = g._entity_handler._NPC_1_TOTAL_CHARS;
		break;
	case NPC_TYPE::NPC2:
		_text = g._entity_handler._NPC_2_TEXT;
		_total_chars = g._entity_handler._NPC_2_TOTAL_CHARS;
		break;
	default:
		std::cout << "ERROR_NPC_TYPE";
		std::cin.get();
		break;
	}
	_temp = _text[0];
	_text[0] = '\0';
}

Npc::~Npc()
{
	_text[_chars_in] = _temp;
	std::cout << "DESTRUCTED!!";

}

bool Npc::logic(Game& g)
{
	// if player close, show "Press E"
	Player& p = g._entity_handler._p;

	if (_talking_to) {

		constexpr float threshold = 400.f;
		if (threshold < abs(get_mid_x() - p.get_mid_x()) + abs(get_mid_y() - p.get_mid_y()))
		{
			_talking_to = false;
		} 
		else if (!_end_of_page)
		{
			_speaking_timer += g._dt;

			int chars_should_be_in = (int)(_speaking_timer) / 45;

			if (chars_should_be_in > _chars_in)
			{
				// go to next character
				_text[_chars_in] = _temp;
				++_chars_in;

				if (_text[_chars_in] == '\0') {
					// at end of page
					_end_of_page = true;
				}

				_temp = _text[_chars_in];
				_text[_chars_in] = '\0';
			}
			else {
				// fast forward
				if (g._keys_frame[SDLK_e]) {
					_speaking_timer += 200.f;
				}
			}
		}
		else {
			// waiting for e press
			_press_e_sign = true;
			if (g._keys_frame[SDLK_e]) {
				if (_chars_in + 1 >= _total_chars) {
					// Done with all pages
					_talking_to = false;
				}
				else {
					// Next page !
					++_chars_in;
					_char_at = _chars_in;
					_end_of_page = false;
					_temp = _text[_char_at];
					_text[_char_at] = '\0';
					_press_e_sign = false;
				}
			}
		}
	}
	else
	{
		constexpr float threshold = 150.f;
		_press_e_sign = threshold > abs(get_mid_x() - p.get_mid_x()) + abs(get_mid_y() - p.get_mid_y());

		if (_press_e_sign) {
			if (g._keys_frame[SDLK_e]) {
				_talking_to = true;
				_press_e_sign = false;
			}
		}
	}

	return false;
}

void Npc::draw(Game& g)
{
	// Sprite and "Press E" box
	{
		bool condition = (!_press_e_sign && _talking_to) && ((((int)_speaking_timer) / 400) % 2 == 0);
		int x_offset = (condition) ?  5 : 0;

		const SDL_Rect rect = { g._cam.convert_x((int)_x) + x_offset, g._cam.convert_y((int)_y),(int)_w,(int)_h };

		draw_circle(g._renderer, rect.x + _w / 2, rect.y + _h * 0.9f, 25, { 0,0,0,67 });

		SDL_RenderCopy(g._renderer, g._textures[TEX::RedBear], NULL, &rect);

		// "Press E" Text
		if (_press_e_sign) {
			constexpr int scale = 3;
			const SDL_Rect text_rect = { rect.x - 25 + (int)(10.f * sinf(((float)g._ticks) * 0.1f)), rect.y - 50, g._press_e_w_and_h[0] * scale, g._press_e_w_and_h[1] * scale };
			SDL_SetRenderDrawColor(g._renderer, 255, 255, 255, 127);
			SDL_RenderFillRect(g._renderer, &text_rect);
			SDL_RenderCopy(g._renderer, g._press_e_texture, NULL, &text_rect);
		}
	}

	// Talking box
	if (_talking_to) {
		SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, 255);
		int y = (int)((float)g._HEIGHT * 0.575f);
		int x = 267;
		int scale = 20;
		const SDL_Rect box = { x, y, 32*scale, 15*scale };
		SDL_RenderCopy(g._renderer, g._textures[TEX::DialogueBox], NULL, &box);


		constexpr int text_offset = 40;
		g._cam.draw_text(g, _text + _char_at, {0,0,0,255}, x + text_offset, y + text_offset, 3);
	}
}

void Npc::intersection(Game& g, float nx, float ny, MovingRect* e)
{
}
