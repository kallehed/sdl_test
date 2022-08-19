#include "BossHead.h"
#include "Game.h"

MOVING_RECT_TYPES BossHead::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::BOSS;
}

BossHead::BossHead(float x, float y)
	: MovingRect(x, y, 50.f, 50.f, 0.1f)
{
}

bool BossHead::logic(Game& g)
{
	return false;
}

void BossHead::draw(Game& g)
{
	SDL_Rect rect = { g._cam.convert_x((int)get_x()), g._cam.convert_y((int)get_y()),(int)(get_w() * 1.2f),(int)(get_h() * 1.2f) };

	SDL_RenderCopyEx(g._renderer, g._textures[TEX::RedHumanBackwards], NULL, &rect, _degrees_turned, NULL, SDL_FLIP_NONE);
}

void BossHead::intersection(Game& g, float nx, float ny, MovingRect* e)
{
}
