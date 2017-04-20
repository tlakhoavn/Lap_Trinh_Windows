#pragma once

#include "resource.h"
#include "ChartControl.h"
#include "DailyExpense.h"

#include <shlwapi.h>

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnPaint(HWND hwnd);
void OnDestroy(HWND hwnd);
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);

void InitChildControl(HWND hWnd, HINSTANCE hInst); //Khởi tạo các control lên màn hình
void AddCriteriaComboboxItem(HWND h_cb_hWnd);                     //Thêm các tiêu chí vào comboboxContent
void LoadHistory();								   //Đọc dữ liệu lịch sử từ file history.txt              
void SavetoHistory(CDailyExpense Expense);			//Thêm dữ liệu vào file theo từng dòng

void OnclickButtonAdd();

BOOL InitListViewColumns(HWND hWndListView);       //Khởi tạo listview
BOOL AddValueToListView(HWND hWndListView, CDailyExpense Expense);        //Thêm dữ liệu vào listview
LPTSTR getCriteriaByID(int ID);                    //Trả về chuỗi tiêu chí dựa vào số thứ tự


//Global var
CChartControl g_chartStatistical;
CDailyExpense g_Expense;
long g_TotalMoney = 0;

int const count_criteria = 6;								//Số lượng hạng mục
int const max_lenght_text = 50;

long	Arr_Money[count_criteria] = { 0 };							//Mảng chứa số tiền của từng mục
WCHAR Criterias[count_criteria][max_lenght_text] =					//Mảng chứa danh sách các hạng mục
{
	TEXT_ANUONG,TEXT_DILAI,TEXT_NHACUA,TEXT_XECO,TEXT_NHUYEUPHAM,TEXT_DICHVU
};


//Ten đường dẫn file lưu trữ lịch sử
#define HISTORY_FILE_PATH "history.out"

//Group box them moi
HWND h_grb_AddNew;
HWND h_cbb_Criteria;
HWND h_txt_Content;
HWND h_txt_Amount_Money;
HWND h_btn_Add;
HWND h_lab_Criteria, h_lab_Content, h_lab_Amount_Money;
//Group box thong tin
HWND h_grb_History;
HWND h_lb_History;
HWND h_lv_History;
//Group box thong ke
HWND h_lab_Statistical;
HWND h_txt_Total_Money;
HWND h_lab_Total_Money;







