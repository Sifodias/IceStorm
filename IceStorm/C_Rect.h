#pragma once

class C_Rect
{
public:
	C_Rect();
	int x, y;
	int w, h;

	//Posay
	int px, py;		//px, py les x et y relatifs a la matrice, mx, my -> manqueX, manqueY
	int mx, my;
	~C_Rect();
};

