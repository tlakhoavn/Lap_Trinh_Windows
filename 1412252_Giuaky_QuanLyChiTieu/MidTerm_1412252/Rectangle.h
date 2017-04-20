#pragma once
class CRectangle
{
	int m_left;
	int m_top;
	int m_right;
	int m_bottom;

	int m_color_R;
	int m_color_G;
	int m_color_B;

	COLORREF m_colorRGB;

public:
	CRectangle();
	~CRectangle();

	int get_left();
	int set_left(int value);
	int get_top();
	int set_top(int value);
	int get_right();
	int set_right(int value);
	int get_bottom();
	int set_bottom(int value);

	/*int get_colorR();
	int set_colorR(int value);
	int get_colorG();
	int set_colorG(int value);
	int get_colorB();
	int set_colorB(int value);*/

	COLORREF get_colorRGB();
	void set_colorRGB(COLORREF color);
};

