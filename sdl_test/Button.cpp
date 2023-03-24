#include "Button.h"
#include "Game.h"
#include "General.h"

void Button::construct(Game& g, int x, int y, const char* text)
{

	SDL_Surface* surface = TTF_RenderText_Solid(g._font, text, {0,0,0,255});

	int scale = 3;
	_rect = { x,y, surface->w * scale, surface->h * scale };

	// now you can convert it into a texture
	_text_texture = SDL_CreateTextureFromSurface(g._renderer, surface);

	SDL_FreeSurface(surface);
}

Button::~Button()
{
	SDL_DestroyTexture(_text_texture);
}

BTN::BTN Button::logic(Game& g)
{
	// left click
	int m_x, m_y;
	g.getMouseState(&m_x, &m_y);

	if (General::general_rect_intersection(
		_rect.x, _rect.y, _rect.w, _rect.h,
		m_x, m_y, 0, 0))
	{
		_mouse_held_over = true;
		if (g._mouse_btn_pressed_this_frame[0])
		{
			// clicked on!
			_clicked_on = true;
			return BTN::CLICKED_ON;
		}
		else {
			_clicked_on = false;
		}
		return BTN::HELD_OVER;
	}
	else {
		_mouse_held_over = false;
		_clicked_on = false;
	}

	return BTN::NOTHING;
}

void Button::draw(Game& g)
{
	if (_clicked_on) {
		SDL_SetRenderDrawColor(g._renderer, 50, 50, 50, 255);
	}
	else if (_mouse_held_over) {
		SDL_SetRenderDrawColor(g._renderer, 150, 150, 150, 255);
	}
	else {
		SDL_SetRenderDrawColor(g._renderer, 255, 255, 255, 255);
	}
	SDL_RenderFillRect(g._renderer, &_rect);
	SDL_RenderCopy(g._renderer, _text_texture, NULL, &_rect);
}
