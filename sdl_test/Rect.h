#pragma once

class Rect {
private:
	float _x;
	float _y;
	float _w;
	float _h;
public:
	Rect(float x, float y, float w, float h);
	void set_x(float x);
	void set_y(float y);
	void set_w(float w);
	void set_h(float h);
	float get_x() const;
	float get_y() const;
	float get_mid_x() const;
	float get_mid_y() const;
	float get_w() const;
	float get_h() const;
	float get_half_w() const;
	float get_half_h() const;
};