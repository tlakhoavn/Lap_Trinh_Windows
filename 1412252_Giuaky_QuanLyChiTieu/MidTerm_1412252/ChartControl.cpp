#include "stdafx.h"
#include "ChartControl.h"

#include <commctrl.h>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")
#pragma comment (lib,"Gdiplus.lib")

//So do cac loai chi tieu
HWND h_lab_Food, h_lab_Travel, h_lab_Housing, h_lab_Vehicle, h_lab_Necessity, h_lab_Service;


CChartControl::CChartControl()
{
	m_left = 30;
	m_right = 580;
	m_top = 370;
	m_bottom = m_top + 60;		
}


CChartControl::~CChartControl()
{
}

int CChartControl::get_left() {
	return m_left;
}
int CChartControl::set_left(int value) {
	m_left = value;
	return 0;
}
int CChartControl::get_top() {
	return m_top;
}
int CChartControl::set_top(int value) {
	m_top = value;
	return 0;
}
int CChartControl::get_right() {
	return m_right;
}
int CChartControl::set_right(int value) {
	m_right = value;
	return 0;
}
int CChartControl::get_bottom() {
	return m_bottom;
}
int CChartControl::set_bottom(int value) {
	m_bottom = value;
	return 0;
}

void CChartControl::DrawChart(HDC hdc, long Arr_Value[])
{
	int sum = 0;
	int percen[count_of_rectangle] = { 0 };
	
	//Tạo bảng chú thích cho biểu đồ------------------------.
	ChartNote(hdc);

	//Tạo biểu đồ --------------------------------------------
	for (int i = 0; i < count_of_rectangle; i++)
	{
		sum += Arr_Value[i];
	}

	
	if (0 == sum) {
		sum = 1; //Trường hợp chưa có dữ liệu sẽ ko bị lỗi chia cho 0		
		for (int i = 0; i < count_of_rectangle; i++)
		{
			percen[i] =100/ count_of_rectangle;	
		}		
	}
	else
	{		
		
		for (int i = 0; i < count_of_rectangle; i++)
		{
			percen[i] = Arr_Value[i] * 100 / sum;			
		}
	}
		
	//====================================================================================
	HPEN hPen = CreatePen(PS_NULL, 1, RGB(0, 0, 0));
	HPEN holdPen = (HPEN)SelectObject(hdc, hPen);		
	

	//Xác định vị trí của các hình chữ nhật tổng thể và khởi tạo vị trí các hình chữ nhật con
	int widthChart = m_right - m_left;

	m_GroupRectangle[0].set_left(m_left);
	m_GroupRectangle[0].set_top(m_top);
	m_GroupRectangle[0].set_right(percen[0]*widthChart / 100 + m_left);
	m_GroupRectangle[0].set_bottom(m_bottom);

	for (int i = 1; i < count_of_rectangle; i++)
	{
		m_GroupRectangle[i].set_left(m_GroupRectangle[i - 1].get_right()); // lề trái của phần từ A1 là lề phải phần tử A0
		m_GroupRectangle[i].set_top(m_top);
		m_GroupRectangle[i].set_right(percen[i] * widthChart/100 + m_GroupRectangle[i].get_left());
		m_GroupRectangle[i].set_bottom(m_bottom);
	}

	//Tạo màu cho các HCN con
	hBrush[0] = CreateSolidBrush(RGB(0,255,64));
	hBrush[1] = CreateSolidBrush(RGB(0,255,255));
	hBrush[2] = CreateSolidBrush(RGB(0,64,128));
	hBrush[3] = CreateSolidBrush(RGB(255,0,128));
	hBrush[4] = CreateSolidBrush(RGB(255,255,0));
	hBrush[5] = CreateSolidBrush(RGB(0,128,0));

	HBRUSH 	holdBrush = (HBRUSH)SelectObject(hdc, hBrush[0]);
	for (int i = 0; i < count_of_rectangle; i++)
	{
		SelectObject(hdc, hBrush[i]);
		Rectangle(hdc, -1+m_GroupRectangle[i].get_left(), m_GroupRectangle[i].get_top(),
			m_GroupRectangle[i].get_right(), m_GroupRectangle[i].get_bottom());
	}	

	SelectObject(hdc, holdPen);
	SelectObject(hdc, holdBrush);

	//Giải phóng
	DeleteObject(hPen);
	for (int i = 0; i < count_of_rectangle; i++) {
		DeleteObject(hBrush[i]);
	}	

	
	//Show % theo text
	//====================================================================================
	WCHAR percen_buff[50];
	wsprintf(percen_buff, L"Ăn uống %d%%", percen[0]);
	SetWindowText(h_lab_Food, percen_buff);

	wsprintf(percen_buff, L"Đi lại %d%%", percen[1]);
	SetWindowText(h_lab_Travel, percen_buff);

	wsprintf(percen_buff, L"Nhà cửa %d%%", percen[2]);
	SetWindowText(h_lab_Housing, percen_buff);

	wsprintf(percen_buff, L"Xe cộ %d%%", percen[3]);
	SetWindowText(h_lab_Vehicle, percen_buff);

	wsprintf(percen_buff, L"Nhu yếu phẩm %d%%", percen[4]);
	SetWindowText(h_lab_Necessity, percen_buff);

	wsprintf(percen_buff, L"Dịch vụ %d%%", percen[5]);
	SetWindowText(h_lab_Service, percen_buff);

	//Show %theo biểu đồ
	return;
}


//Tạo bảng chú thích cho biểu đồ
void CChartControl::ChartNote(HDC hdc)
{
	HPEN hPen = CreatePen(PS_NULL, 1, RGB(0, 0, 0));
	HPEN holdPen = (HPEN)SelectObject(hdc, hPen);
	CRectangle GroupRectangle[6];

	const int top = 450;
	const int bottom = top+10;
	const int left = 30;
	

	GroupRectangle[0].set_left(left);
	GroupRectangle[0].set_top(top);
	GroupRectangle[0].set_right(left+10);
	GroupRectangle[0].set_bottom(bottom);

	for (int i = 1; i < count_of_rectangle; i++)
	{
		GroupRectangle[i].set_left(GroupRectangle[i - 1].get_right()+80); // lề trái của phần từ A1 là lề phải phần tử A0
		GroupRectangle[i].set_top(top);
		GroupRectangle[i].set_right(GroupRectangle[i].get_left()+10);
		GroupRectangle[i].set_bottom(bottom);
	}

	//Tạo màu cho các HCN con
	hBrush[0] = CreateSolidBrush(RGB(0, 255, 64));
	hBrush[1] = CreateSolidBrush(RGB(0, 255, 255));
	hBrush[2] = CreateSolidBrush(RGB(0, 64, 128));
	hBrush[3] = CreateSolidBrush(RGB(255, 0, 128));
	hBrush[4] = CreateSolidBrush(RGB(255, 255, 0));
	hBrush[5] = CreateSolidBrush(RGB(0, 128, 0));

	HBRUSH 	holdBrush = (HBRUSH)SelectObject(hdc, hBrush[0]);
	for (int i = 0; i < count_of_rectangle; i++)
	{
		SelectObject(hdc, hBrush[i]);
		Rectangle(hdc, GroupRectangle[i].get_left(), GroupRectangle[i].get_top(),
			GroupRectangle[i].get_right(), GroupRectangle[i].get_bottom());
	}

	SelectObject(hdc, holdPen);
	SelectObject(hdc, holdBrush);

	//Giải phóng
	DeleteObject(hPen);
	for (int i = 0; i < count_of_rectangle; i++) {
		DeleteObject(hBrush[i]);
	}
	return;
}

//Các nhãn %
void CChartControl::InitChartNote(HWND hWnd, HINSTANCE hInst)
{
	//=============================Chú thích biểu đồ=============
	// Get system font
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont2 = CreateFont(15, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	int const note_left = 40;
	int const note_top = 448;
	int const note_widht = 90;
	//Ăn uống
	h_lab_Food = CreateWindowEx(0, L"STATIC", L"Ăn uống",
		WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
		note_left + note_widht * 0, note_top, 80, 20, hWnd, NULL, hInst, NULL);
	SendMessage(h_lab_Food, WM_SETFONT, WPARAM(hFont2), TRUE);

	//Đi lại
	h_lab_Travel = CreateWindowEx(0, L"STATIC", L"Đi lại",
		WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
		note_left + note_widht * 1, note_top, 80, 20, hWnd, NULL, hInst, NULL);
	SendMessage(h_lab_Travel, WM_SETFONT, WPARAM(hFont2), TRUE);

	//Nhà cửa
	h_lab_Housing = CreateWindowEx(0, L"STATIC", L"Nhà cửa",
		WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
		note_left + note_widht * 2, note_top, 80, 20, hWnd, NULL, hInst, NULL);
	SendMessage(h_lab_Housing, WM_SETFONT, WPARAM(hFont2), TRUE);

	//Xe cộ
	h_lab_Vehicle = CreateWindowEx(0, L"STATIC", L"Xe cộ",
		WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
		note_left + note_widht * 3, note_top, 80, 20, hWnd, NULL, hInst, NULL);
	SendMessage(h_lab_Vehicle, WM_SETFONT, WPARAM(hFont2), TRUE);

	//Nhu yếu phẩm
	h_lab_Necessity = CreateWindowEx(0, L"STATIC", L"Nhu yếu phẩm",
		WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
		note_left + note_widht * 4, note_top, 80, 40, hWnd, NULL, hInst, NULL);
	SendMessage(h_lab_Necessity, WM_SETFONT, WPARAM(hFont2), TRUE);

	//Dịch vụ
	h_lab_Service = CreateWindowEx(0, L"STATIC", L"Dịch vụ",
		WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
		note_left + note_widht * 5, note_top, 80, 20, hWnd, NULL, hInst, NULL);
	SendMessage(h_lab_Service, WM_SETFONT, WPARAM(hFont2), TRUE);
}


