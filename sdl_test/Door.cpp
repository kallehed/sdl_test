#include "Door.h"
#include "Game.h"

MOVING_RECT_TYPES Door::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::DOOR;
}

Door::Door(float x, float y, int onetime_index, DOOR_TYPE type)
	: MovingRect(x, y, 32.f*3.f, 53.f*3.f, 0.f),
	_onetime_index(onetime_index),
	_type(type)
{
	using enum DOOR_TYPE;
	switch (_type) {
	case OH_HEY:
		_descriptive_text = "Oh hey";
		break;
	case MORE_DIALOGUE:
		_descriptive_text = "I'm in quite a hurry,\nbut it's REALLY important\nthat you know that...";
		break;
	}
}

bool Door::logic(Game& g)
{
	Player& p = g._entity_handler._p;

	move_and_collide<false>(g);

	if (!_opened)
	{
		// block player from passing through
		if (General::rect_intersection(*((MovingRect*)this), *((MovingRect*)(&p)))) {
			float nx, ny;
			General::normalize_vector_two_points(nx, ny, _x, _y, p._x, p._y);
			float bounce = 0.05f;
			p.change_x_vel(nx * bounce);
			p.change_y_vel(ny * bounce);
		}
		
		float player_leftness = mid_x() - p.mid_x();
		float y_dif = std::abs(mid_y() - p.mid_y());
		_show_e_sign = 0 < player_leftness && player_leftness < 150.f && y_dif < 150.f;

		if (_show_e_sign) {
			if (g._keys_frame[SDL_SCANCODE_E])
			{
				g._save._onetimes.insert({ g._level, _onetime_index });
				_opened = true;
				_opened_text = true;
				_show_e_sign = false;
				return false;
			}
		}
	}
	else
	{
		// state of being opened

		change_y_vel(-0.001f); // FLY TO THE MOON

		// close text when far away
		if (_opened_text) {
			if (abs(y_vel()) > 1.f) {
				_opened_text = false;
			}
		}
	}

	return false;
}
void Door::draw(Game& g)
{
	// Sprite and "Press E" box
	{
		const SDL_Rect rect = { g._cam.convert_x((int)_x), g._cam.convert_y((int)_y),(int)_w,(int)_h };

		draw_circle(g._renderer, rect.x + _w / 2, rect.y + _h * 0.9f, 25, { 0,0,0,67 });

		SDL_RenderCopy(g._renderer, g._textures[TEX::GuyBlonde], NULL, &rect);

		// "Press E" Text
		if (_show_e_sign) {
			constexpr int scale = 3;
			const SDL_Rect text_rect = { rect.x - 25 + (int)(10.f * sinf(((float)g._ticks) * 0.1f)), rect.y - 50, g._press_e_w_and_h[0] * scale, g._press_e_w_and_h[1] * scale };
			SDL_SetRenderDrawColor(g._renderer, 255, 255, 255, 127);
			SDL_RenderFillRect(g._renderer, &text_rect);
			SDL_RenderCopy(g._renderer, g._press_e_texture, NULL, &text_rect);
		}

		// Succeded
		if (_opened_text) {
			SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, 255);
			int y = (int)((float)g._HEIGHT * 0.575f);
			int x = 267;
			int scale = 20;
			const SDL_Rect box = { x, y, 32 * scale, 15 * scale };
			SDL_RenderCopy(g._renderer, g._textures[TEX::DialogueBox], NULL, &box);

			constexpr int text_offset = 40;
			g._cam.draw_text(g, _descriptive_text, { 0,0,0,255 }, x + text_offset, y + text_offset, 3);
		}
	}
}

void Door::intersection(Game& g, float nx, float ny, MovingRect* e)
{
}
