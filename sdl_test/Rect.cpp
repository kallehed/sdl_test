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

float Rect::x() const
{
	return _x;
}

float Rect::y() const
{
	return _y;
}

float Rect::mid_x() const
{
	return _x + _w/2.f;
}
float Rect::mid_y() const
{
	return _y + _h/2.f;
}

float Rect::w() const
{
	return _w;
}

float Rect::h() const
{
	return _h;
}

float Rect::half_w() const
{
	return _w/2.f;
}

float Rect::half_h() const
{
	return _h/2.f;
}
