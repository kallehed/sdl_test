#include "Enemy.h"
#include "Game.h"
#include "General.h"
#include "Coin.h"

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

	// kill self?
	if (_hp < 1) {
		// spawn coins?
		g._entity_handler._entities_to_add.push_back(new Coin(get_mid_x(),get_mid_y(), get_x_vel()/20.f, get_y_vel()/20.f));

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
