#pragma once
#include "MovingRect.h"
#include <string>

class Game;

class Portal : public MovingRect
{
public:
	bool _player_close = false;

	int _destination_level;

	std::string _name;
	std::string _destination_name;

	MOVING_RECT_TYPES get_moving_rect_type() const override final;

	Portal(float x, float y, int destination_level, std::string name, std::string destination_name);

	bool logic(Game& g) override;

	void draw(Game& g) override;

	void intersection(Game& g, float nx, float ny, MovingRect* e) override;

};

