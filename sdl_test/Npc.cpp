#include "Npc.h"
#include "Game.h"
#include "General.h"

MOVING_RECT_TYPES Npc::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::NPC;
}

Npc::Npc(float x, float y)
	: MovingRect(x, y, 70, 80, 1.f)
{
	
}

bool Npc::logic(Game& g)
{
	// if player close, show "Press E"
	Player& p = g._entity_handler._p;

	if (_talking_to)
	{
		_speaking_timer += g._dt;

		if (_text_slide_done)
		{
			_player_close = true;
			if (g._keys_frame[SDLK_e]) {
				if (_text_slide + 1 == _texts.size())
				{
					_talking_to = false;
					_speaking_timer = 0.f;
					_text_slide_done = false;
				}
				else
				{
					++_text_slide;
					_speaking_timer = 0.f;
					_text_slide_done = false;
					_player_close = false;
				}
			}
		}
		else
		{
			if (g._keys_frame[SDLK_e])
			{
				_speaking_timer += 200.f; // add 100 ms.
			}
		}
	}
	else
	{
		constexpr float threshold = 150.f;
		_player_close = threshold > abs(get_mid_x() - p.get_mid_x()) + abs(get_mid_y() - p.get_mid_y());

		if (_player_close) {
			if (g._keys_frame[SDLK_e]) {
				_talking_to = true;
				_player_close = false;
			}
		}
	}

	return false;
}

void Npc::draw(Game& g)
{
	// Sprite and "Press E" box
	{
		const SDL_Rect rect = { g._cam.convert_x((int)_x), g._cam.convert_y((int)_y),(int)_w,(int)_h };

		SDL_RenderCopy(g._renderer, g._textures[TEX::RedBear], NULL, &rect);

		// "Press E" Text
		if (_player_close) {
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

		int characters = (int)(_speaking_timer / 90.f);
		std::string sub_str = _texts[_text_slide].substr(0, characters);
		if (characters >= _texts[_text_slide].length()) {
			_text_slide_done = true;
		}
		int text_offset = 40;
		g._cam.draw_text(g, sub_str.c_str(), {0,0,0,255}, x + text_offset, y + text_offset, 3);
	}
}

void Npc::intersection(Game& g, float nx, float ny, MovingRect* e)
{
}
