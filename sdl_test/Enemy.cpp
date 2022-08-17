#include "Enemy.h"
#include "Game.h"
#include "General.h"
#include "Pickupable.h"
#include "Particle.h"
#include "Explosion.h"
#include "FireMagic.h"
#include "Shot.h"

Enemy::Enemy(float x, float y,float w, float h, int max_hp, float activation_radius, float deactivation_radius, float active_time) : MovingRect(x, y, w, h, 0.01f),
	_max_hp(max_hp), _hp(max_hp), _activation_radius_squared(activation_radius*activation_radius),
	_deactivation_radius_squared(deactivation_radius*deactivation_radius), _active_time(active_time)
{
	make_idle();
}

bool Enemy::logic(Game& g)
{
	// handle phases
	if (!_active) // idle phase
	{
		idle_logic(g);

		// potentially switch phase
		if (in_radius_squared_of_player(g, _activation_radius_squared))
		{
			make_active();
		}
	}
	else { // active phase
		
		active_logic(g);

		_active_timer += g._dt;
		if (_active_timer > _active_time) // return to idle phase
		{ 
			make_idle();
		}
		else if (in_radius_squared_of_player(g, _deactivation_radius_squared)) // keep being in attack phase
		{ 
			_active_timer = 0.f;
		}
	}

	move_and_collide<true>(g);

	// particles
	{
		if (abs(get_x_vel()) + abs(get_y_vel()) >= 0.05f && g._ticks % (20 + rand() % 11) == 0) {
			g._entity_handler._particles.emplace_back(new Particle(get_mid_x(), get_mid_y(), -get_x_vel(), -get_y_vel(), { 0, 200, 255, 175 }));
		}
	}

	// kill self?
	if (_hp < 1) {
		// spawn coins?
		int coins = 1 + rand() % 2;
		for (int _ = 0; _ < coins; ++_) {
			g._entity_handler._entities_to_add.push_back(new Pickupable(PICKUPABLE_TYPE::COIN, get_mid_x() + _, get_mid_y() + _, get_x_vel() / 20.f, get_y_vel() / 20.f));
		}
		delete this;
		return true;
	}
	return false;
}

void Enemy::make_active()
{
	_active = true;
	_active_timer = 0.f;
}

void Enemy::make_idle() 
{
	_active = false;
}

void Enemy::go_towards_player(Game& g, float speed)
{
	//std::cout << "s";
	this->go_towards(g._entity_handler._p.get_mid_x(), g._entity_handler._p.get_mid_y(), speed);
}

bool Enemy::in_radius_squared_of_player(Game& g, float radius_squared)
{
	float dx = get_mid_x() - g._entity_handler._p.get_mid_x();
	float dy = get_mid_y() - g._entity_handler._p.get_mid_y();
	return dx * dx + dy * dy <= radius_squared;
}

void Enemy::new_walk_path(Game& g) // clear
{
	_path_progress = 1;
	_walk_path = General::generate_walk_path(g, get_mid_x(), get_mid_y(), g._entity_handler._p.get_mid_x(), g._entity_handler._p.get_mid_y());
}

void Enemy::intersection(Game& g, float nx, float ny, MovingRect* e)
{
	switch (e->get_moving_rect_type()) {
	case MOVING_RECT_TYPES::SHOT:
	{
		if (((Shot*)e)->_owner == this) { // can't shoot self(not instantly anyway)
			break;
		}
		float bounce_acc = 0.05f;

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);

		take_damage();

		make_active(); // become active (aggressive)

		break;
	}
	case MOVING_RECT_TYPES::ENEMY:
	{
		float bounce_acc = 0.005f;

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);
		break;
	}
	case MOVING_RECT_TYPES::EXPLOSION:
	{
		float bounce_acc = 0.1f;

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);

		this->take_damage(((Explosion*)(e))->_damage);
		this->make_active();
		break;
	}
	case MOVING_RECT_TYPES::FIRE_MAGIC:
	{
		float bounce_acc = 0.05f;

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);

		take_damage(((FireMagic*)(e))->_damage);
		make_active();

		break;
	}
	}

}
