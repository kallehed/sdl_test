#include "BossBody.h"
#include "Game.h"
#include "Shot.h"
#include "Explosion.h"
#include "FireMagic.h"
#include "Particle.h"

MOVING_RECT_TYPES BossBody::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::BOSS;
}

BossBody::BossBody(float x, float y, int more_to_add, BossBody* next)
	: MovingRect(x, y, 50.f, 50.f, 0.01f),
	_more_to_add(more_to_add),
	_next(next)
{
}

bool BossBody::logic(Game& g)
{
	auto& p = g._entity_handler._p;

	// check hp situation
	if (_hp <= 0) {
		if (_back != nullptr) {
			_back->_next = _next;
		}
		if (_next != nullptr) {
			_next->_back = _back;
		}

		delete this;
		return true;
	}

	if (_more_to_add > 0)
	{
		BossBody* e = new BossBody(_x - 100, _y, _more_to_add - 1, this);
		_back = e;
		g._entity_handler._entities_to_add.emplace_back(e);

		_more_to_add = 0;
	}
	

	MovingRect* target = (_next == nullptr) ?
		static_cast<MovingRect*>(&g._entity_handler._p) : static_cast<MovingRect*>(_next);

	// turn towards target
	{
		float r = atan2(target->get_mid_y() - get_mid_y(), target->get_mid_x() - get_mid_x()); // SECOND HALF IS NEGATIVE FOR SOME REASON???

		if (r < 0.f) { r += General::tau(); } // BECAUSE OF THAT, THIS

		float r_cor = General::normalize_radian(r - _radians_turned);

		constexpr float turn = 0.024f;

		if (r_cor > General::pi()) {
			_radians_turned -= turn;
		}
		else {
			_radians_turned += turn;
		}

	}
	_radians_turned = General::normalize_radian(_radians_turned);

	_x_dir = cosf(_radians_turned);
	_y_dir = sinf(_radians_turned);

	float speed = true  ? 0.0013f : 0.003f;
	change_x_vel(_x_dir * speed);
	change_y_vel(_y_dir* speed);

	move_and_collide<true>(g);

	return false;
}

void BossBody::draw(Game& g)
{
	SDL_Rect rect = { g._cam.convert_x((int)_x), g._cam.convert_y((int)_y),(int)_w,(int)_h };

	draw_circle(g._renderer, rect.x + _w / 2, rect.y + _h * 0.9f, 25, { 0,0,0,67 });

	TEX::TEX tex = (_next != nullptr) ? TEX::BossBody : TEX::BossHead;

	double rot = (double)General::radians_to_degrees(_radians_turned);

	SDL_RenderCopyEx(g._renderer, g._textures[tex], NULL, &rect, rot, NULL, SDL_FLIP_NONE);

}

void BossBody::intersection(Game& g, float nx, float ny, MovingRect* e)
{
	int damage = 0;
	float bounce_acc = 0.f;

	switch (e->get_moving_rect_type()) {
	case MOVING_RECT_TYPES::BOSS:
	{
		bounce_acc = 0.0075f;
		break;
	}
	case MOVING_RECT_TYPES::SHOT:
	{
		if (((Shot*)e)->_owner == this) { // can't shoot self(not instantly anyway)
			break;
		}
		bounce_acc = 0.05f;

		damage = 5;
		break;
	}
	case MOVING_RECT_TYPES::EXPLOSION:
	{
		bounce_acc = 0.1f;

		damage = ((Explosion*)e)->_damage;
		break;
	}
	case MOVING_RECT_TYPES::FIRE_MAGIC:
	{
		bounce_acc = 0.05f;

		damage = ((FireMagic*)e)->_damage;
		break;
	}
	}

	if (bounce_acc != 0.f) {
		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);
	}

	if (damage > 0) {
		this->take_damage(g, damage);

		// shake screen
		g._cam.shake(g, 1.3f, 10.f);

		// particles
		{
			if (_hp > 0) {
				float x_vel = nx;
				float y_vel = ny;
				int total_particles = damage;
				for (int i = 0; i < total_particles; ++i) {
					Particle* e = new Particle(get_mid_x(), get_mid_y(), x_vel * General::randf01(), y_vel * General::randf01(), { 0, 200, 255, 255 });
					g._entity_handler._particles.emplace_back(e);
				}
			}
			else {
				float x_vel = nx * 3.f;
				float y_vel = ny * 3.f;
				int total_particles = 50;
				for (int i = 0; i < total_particles; ++i) {
					Particle* e = new Particle(get_mid_x(), get_mid_y(), x_vel * (0.25f + 0.75f * General::randf01()), y_vel * (0.25f + 0.75f * General::randf01()), { 0, 200, 255, 255 });
					g._entity_handler._particles.emplace_back(e);
				}
			}
		}
	}
}

void BossBody::take_damage(Game& g, int damage)
{
	_hp -= damage;
}
