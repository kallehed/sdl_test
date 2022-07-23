#include "Player.h"
#include "Game.h"
#include "Bomb.h"

MOVING_RECT_TYPES Player::get_moving_rect_type() const
{
	return MOVING_RECT_TYPES::PLAYER;
}

Player::Player() : MovingRect(0.f, 0.f, 40.f, 40.f, 0.009f) {
}

bool Player::logic(Game& g)
{
	// change velocities according to keys pressed
	static const float acc = 0.0023f;
	const Uint8* keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
		change_x_vel(acc);
	}
	if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) {
		change_x_vel(-acc);
	}
	if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S]) {
		change_y_vel(acc);
	}
	if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) {
		change_y_vel(-acc);
	}

	// let MovingRect handle the rest
	move_and_collide(g);

	// shoot
	_shoot_timer += g._dt;
	if (_shoot_timer > _shoot_time)
	{
		int x, y;
		Uint32 buttons = SDL_GetMouseState(&x, &y);
		if (((buttons & SDL_BUTTON_LMASK) != 0)) // left
		//if (g._mouse_btn_pressed_this_frame[0]) // left mouse btn
		{
			_shoot_timer = 0.f;

			// shoot projectile "Shot"
			float shot_speed = 0.5f;
			float nx, ny;
			General::normalize_vector_two_points(nx, ny, g._cam.convert_x(get_mid_x()), g._cam.convert_y(get_mid_y()), (float)g._mouse_pos_on_latest_press[0], (float)g._mouse_pos_on_latest_press[1]);

			float x_speed = nx * shot_speed;
			float y_speed = ny * shot_speed;
			g._entity_handler._entities_to_add.push_back(new Shot(get_mid_x(), get_mid_y(), x_speed, y_speed));
		}
	}

	// throw bomb
	if (g._mouse_btn_pressed_this_frame[2]) // right mouse => bomb
	{
		float bomb_speed = 1.5f;
		float nx, ny;
		General::normalize_vector_two_points(nx, ny, g._cam.convert_x(get_mid_x()), g._cam.convert_y(get_mid_y()), (float)g._mouse_pos_on_latest_press[0], (float)g._mouse_pos_on_latest_press[1]);

		float x_vel = nx * bomb_speed;
		float y_vel = ny * bomb_speed;

		g._entity_handler._entities_to_add.push_back(new Bomb(get_mid_x(), get_mid_y(), x_vel, y_vel));
	}
	return false;
}


void Player::draw(Game& g)
{
	SDL_SetRenderDrawColor(g._renderer, 0, 0, 0, 255);

	SDL_Rect rect = { g._cam.convert_x((int)get_x()), g._cam.convert_y((int)get_y()),(int)get_w(),(int)get_h() };
	SDL_RenderDrawRect(g._renderer, &rect);
}

void Player::intersection(float nx, float ny, MovingRect* e)
{
	switch (e->get_moving_rect_type())
	{
	case MOVING_RECT_TYPES::ENEMY: // handle bumping into enemy
	{
		float bounce_acc = 0.05f;

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);

		_hp -= 5;
		break;
	}
	case MOVING_RECT_TYPES::SHOT_ENEMY:
	{
		float bounce_acc = 0.05f;

		change_x_vel(bounce_acc * nx);
		change_y_vel(bounce_acc * ny);

		_hp -= 5;
		break;
	}
	case MOVING_RECT_TYPES::COIN:
	{
		_coins += 1;
		break;
	}
	}
}
