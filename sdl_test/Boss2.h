#pragma once
#include "MovingRect.h"
#include "General.h"

class Boss2 : public MovingRect
{
public:
	int _onetime_index;

	BOSS2_STATE _state = BOSS2_STATE::WAITING_FOR_PLAYER;

	static constexpr int _max_hp = 200;
	int _hp = _max_hp;

	int _damage = 5;

	float _timer = DEV::DEV ? 10000.f : 0.f;

	union {
		struct {
			bool _SHOOTING_has_shot;
		};
		struct {
			bool _SPAWNING_has_spawned;
		};
		struct {
			bool _RANDOM_WALK_has_changed_dir;
			float _RANDOM_WALK_x_dir;
			float _RANDOM_WALK_y_dir;
		};
		struct {
			bool _BOMBING_has_thrown_bomb;
		};
	};

	MOVING_RECT_TYPES get_moving_rect_type() const override;

	// for head
	Boss2(int onetime_index, float x, float y);

	bool logic(Game& g) override;

private:
	void set_state_to_deciding(Game& g);
public:

	void draw(Game& g) override;

	void intersection(Game& g, float nx, float ny, MovingRect* e) override;

	void take_damage(Game& g, int damage);
};

