#pragma once

class C_Rect
{
public:
	C_Rect(double ix = 0, double iy = 0, int iw = 0, int ih = 0) :
		x(ix), y(iy), w(iw), h(ih) {};
	C_Rect(C_Rect& copy) {
		x = copy.x;
		y = copy.y;
		w = copy.w;
		h = copy.h;
	}
	bool operator==(C_Rect reqt){
		return x == reqt.x && y == reqt.y && w == reqt.w && h == reqt.h;
	}
	double x, y;
	int w, h;
};

