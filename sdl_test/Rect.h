#pragma once

class Rect {
public:
	float _x;
	float _y;
	float _w;
	float _h;

	Rect(float x, float y, float w, float h);
	void set_x(float x);
	void set_y(float y);
	void set_w(float w);
	void set_h(float h);
	float x() const;
	float y() const;
	float mid_x() const;
	float mid_y() const;
	float w() const;
	float h() const;
	float half_w() const;
	float half_h() const;
};