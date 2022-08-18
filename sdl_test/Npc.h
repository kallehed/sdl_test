#pragma once
#include <string>
#include <array>
#include "MovingRect.h"
#include "enums.h"

class Npc : public MovingRect
{
public:
	NPC_TYPE _npc_type;

	// whether visible or not
	bool _press_e_sign = false;

	bool _talking_to = false; // to player

	TEX::TEX _tex = TEX::RedHumanBackwards; // Npc texture

	char* _text; // ex "hello\nperson\0I like bread\0"
	int _total_chars = 0; // in _text

	float _speaking_timer = 0.f;
	int _char_at = 0; // where to start displaying text
	int _chars_in = 0; // at what character \0 is placed.
	char _temp = 'Z'; // temporary
	bool _end_of_page = false; // whether at end of page or not

	// Which red bear should spawn
	static int _red_bear_stage;

	bool _invisible = false;

	MOVING_RECT_TYPES get_moving_rect_type() const override final;

	Npc(Game& g, NPC_TYPE type, float x, float y);
	
	~Npc();

	bool logic(Game& g) override;

	void draw(Game& g) override;

	void intersection(Game& g, float nx, float ny, MovingRect* e) override;
};

