#include "Portal.h"
#include "Game.h"

MOVING_RECT_TYPES Portal::get_moving_rect_type() const
{
    return MOVING_RECT_TYPES::PORTAL;
}

Portal::Portal(float x, float y, int destination_level, std::string name, std::string destination_name)
	: MovingRect(x, y, 64, 81, 1.f), _destination_level(destination_level), _name(name), _destination_name(destination_name)
{
}

bool Portal::logic(Game& g)
{
	Player& p = g._entity_handler._p;

	constexpr float threshold = 150.f;
	_player_close = threshold > abs(get_mid_x() - p.get_mid_x()) + abs(get_mid_y() - p.get_mid_y());

	if (_player_close) {
		if (g._keys_frame[SDL_SCANCODE_E])
		{
			// teleport to other level
			g._level_to_change_to = _destination_level;
			g._destination_portal = _destination_name;
			g._change_level = true;
		}
	}

    return false;
}

void Portal::draw(Game& g)
{
	// Sprite and "Press E" box
	{
		const SDL_Rect rect = { g._cam.convert_x((int)_x), g._cam.convert_y((int)_y),(int)_w,(int)_h };

		SDL_RenderCopy(g._renderer, g._textures[TEX::Portal], NULL, &rect);

		// "Press E" Text
		if (_player_close) {
			constexpr int scale = 3;
			const SDL_Rect text_rect = { rect.x - 25 + (int)(10.f * sinf(((float)g._ticks) * 0.1f)), rect.y - 50, g._press_e_w_and_h[0] * scale, g._press_e_w_and_h[1] * scale };
			SDL_SetRenderDrawColor(g._renderer, 255, 255, 255, 127);
			SDL_RenderFillRect(g._renderer, &text_rect);
			SDL_RenderCopy(g._renderer, g._press_e_texture, NULL, &text_rect);
		}
	}
}

void Portal::intersection(Game& g, float nx, float ny, MovingRect* e)
{
}
