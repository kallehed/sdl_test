#include "EnemyShooter.h"
#include "Game.h"
#include "Shot.h"
#include "Explosion.h"

MOVING_RECT_TYPES EnemyShooter::get_moving_rect_type() const
{
    return MOVING_RECT_TYPES::ENEMY;
}

EnemyShooter::EnemyShooter(float x, float y)
	: Enemy(x, y,45.f,45.f, 20, 200.f, 300.f, 5000.f, 0.0015f, 2000.f, 3000.f,
		250.f - 50 * General::randf01(), 300.f - 50 * General::randf01(), 0.0011f)
{
}

void EnemyShooter::draw(Game& g)
{
	SDL_Rect rect = { g._cam.convert_x((int)x()), g._cam.convert_y((int)y()),(int)(_w*1.2f),(int)(_h*1.2f) };

	draw_circle(g._renderer, rect.x + _w / 2, rect.y + _h * 0.9f, 20, { 0,0,0,67 });

	SDL_RendererFlip flip = (x_vel() > 0.f) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;

	if (_hurt_timer > 0.f) {
		SDL_SetTextureColorMod(g._textures[TEX::Wizard], 100, 100, 100);
		_hurt_timer -= g._dt;
	}
	else {
		SDL_SetTextureColorMod(g._textures[TEX::Wizard], 255, 255, 255);
	}

	SDL_RenderCopyEx(g._renderer, g._textures[TEX::Wizard], NULL, &rect, NULL, NULL, flip);
}

void EnemyShooter::active_logic(Game& g)
{
	_shoot_timer += g._dt;

	if (_shoot_timer > 750.f)
	{
		_shoot_timer = 0.f + 200.f*(General::randf01() - 0.5f);
		// shoot projectile "Shot"
		float speed = 0.028f;
		float nx, ny;
		Player& p = g._entity_handler._p;
		General::normalize_vector_two_points(nx, ny, mid_x(), mid_y(), p.mid_x(), p.mid_y());

		Shot* new_shot = new Shot(this, 5, mid_x(), mid_y(), nx, ny, speed, 1, TEX::MagicOrb);
		g._entity_handler._entities_to_add.push_back(new_shot);
	}

	stay_in_range_of_player(g);
}