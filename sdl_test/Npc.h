#pragma once
#include <string>
#include "MovingRect.h"

class Game;

class Npc : public MovingRect
{
public:

	bool _player_close = false;

	bool _talking_to = false;

	float _speaking_timer = 0.f;
	int _text_slide = 0;
	bool _text_slide_done = false;

	std::array<std::string, 6> _texts = {
		"Hello traveller...\nYou are sure to be confused,\nbut do not worry!",
		"I am, The Read Bear.\nThe great helper!",
		"Firstly...\nAs you probably already know,\nyou can move with the arrows",
		"Secondly, with the mouse, you\ncan 'left click' to attack!",
		"and possibly also right click\nfor something else...",
		"Thirdly... Nothing can describe\nwhat you are about to\nexperience...",
	};

	MOVING_RECT_TYPES get_moving_rect_type() const override final;

	Npc(float x, float y);

	bool logic(Game& g) override;

	void draw(Game& g) override;

	void intersection(Game& g, float nx, float ny, MovingRect* e) override;
};

