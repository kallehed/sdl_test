#include "SaveState.h"



SaveState::SaveState()
{
	this->reset();
}

void SaveState::reset()
{
	_onetimes.clear();

	_red_bear_stage = 0;

	_sus_seller_stage = 0;
}
