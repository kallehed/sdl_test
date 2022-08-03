#include "Npc.h"
#include "Game.h"

MOVING_RECT_TYPES Npc::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::NPC;
}

Npc::Npc(float x, float y)
	: MovingRect(x, y, 70, 70, 1.f)
{

}

bool Npc::logic(Game& g)
{
	return false;
}

void Npc::draw(Game& g)
{
	const SDL_Rect rect = { g._cam.convert_x((int)_x), g._cam.convert_y((int)_y),(int)_w,(int)_h };

	SDL_RenderCopy(g._renderer, g._textures[TEX::GreenSlime2], NULL, &rect);
}

void Npc::intersection(Game& g, float nx, float ny, MovingRect* e)
{
}
