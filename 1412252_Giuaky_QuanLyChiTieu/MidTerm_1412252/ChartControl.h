#pragma once
#include "Rectangle.h"
#include "Resource.h"

int const count_of_rectangle = 6;
class CChartControl
{
private: 
		int m_left;				//vị trí của biểu đồ
		int m_top;
		int m_right;
		int m_bottom;
		CRectangle m_GroupRectangle[count_of_rectangle];		//6 phần của biểu đồ
		HBRUSH hBrush[count_of_rectangle];
		
public:
	CChartControl();
	~CChartControl();

	int get_left();
	int set_left(int value);
	int get_top();
	int set_top(int value);
	int get_right();
	int set_right(int value);
	int get_bottom();
	int set_bottom(int value);

	void DrawChart(HDC hdc, long Arr_Value[]); //Cập nhật biểu đồ
	void ChartNote(HDC hdc);                           //Tạo phần chú thích cho biểu đồ
	void InitChartNote(HWND hWnd, HINSTANCE hInst);  //Các nhãn %
};

