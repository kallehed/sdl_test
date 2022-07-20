#include "Rect.h"
Rect::Rect(float x, float y, float w, float h)
	: _w(w), _h(h)
{
	set_x(x);
	set_y(y);
}
void Rect::set_x(float x)
{
	_x = x;
}

void Rect::set_y(float y)
{
	_y = y;
}

void Rect::set_w(float w)
{
	_w = w;
}
void Rect::set_h(float h)
{
	_h = h;
}

float Rect::get_x() const
{
	return _x;
}

float Rect::get_y() const
{
	return _y;
}

float Rect::get_mid_x() const
{
	return _x + _w/2.f;
}
float Rect::get_mid_y() const
{
	return _y + _h/2.f;
}

float Rect::get_w() const
{
	return _w;
}

float Rect::get_h() const
{
	return _h;
}

float Rect::get_half_w() const
{
	return _w/2.f;
}

float Rect::get_half_h() const
{
	return _h/2.f;
}
