// MidTerm_1412252.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "MidTerm_1412252.h"

#include <windowsX.h>
#include <Windows.h>
#include <winuser.h>
#include <objidl.h>
#include <gdiplus.h>
#include <commctrl.h>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")
#pragma comment (lib,"Gdiplus.lib")

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")



#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MIDTERM_1412252, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MIDTERM_1412252));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MIDTERM_1412252));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MIDTERM_1412252);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_SYSMENU | WS_OVERLAPPED | WS_VISIBLE | WS_CLIPCHILDREN | WS_MINIMIZEBOX,
      CW_USEDEFAULT, 0, 610, 550, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    { 
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);	
       
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	InitChildControl(hWnd, hInst);
	LoadHistory();
	return TRUE;
}

void OnCommand(HWND hWnd, int wmId, HWND hwndCtl, UINT codeNotify)
{
	switch (wmId)
	{
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	case IDC_BTN_ADD:
		OnclickButtonAdd();
		break;
	}
}

void OnPaint(HWND hWnd)
{	
	
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);	
	g_chartStatistical.DrawChart(hdc, Arr_Money);	
	EndPaint(hWnd, &ps);	
}

void OnDestroy(HWND hwnd)
{
	PostQuitMessage(0);
}

//Khởi tạo các control lên màn hình
void InitChildControl(HWND hWnd, HINSTANCE hInst)
{
	// Get system font
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(18, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);


	//Group thêm mới tiêu chí
	h_grb_AddNew = CreateWindowEx(0, L"BUTTON", L"Thêm một loại tiêu chí",
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		10, 30, 570, 90, hWnd, NULL, hInst, NULL);
	SendMessage(h_grb_AddNew, WM_SETFONT, WPARAM(hFont), TRUE);

	//Tiêu chí
	h_lab_Criteria = CreateWindowEx(0, L"STATIC", L"Loại tiêu chí",
		WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
		20, 50, 120, 20, hWnd, NULL, hInst, NULL);
	SendMessage(h_lab_Criteria, WM_SETFONT, WPARAM(hFont), TRUE);		

		h_cbb_Criteria = CreateWindowEx(0, L"COMBOBOX", L"",
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | CBS_DROPDOWN | CBS_HASSTRINGS | BS_GROUPBOX,
			20, 70, 120, 20, hWnd, (HMENU)IDC_CBB_CRITERIA, hInst, NULL);
		SendMessage(h_cbb_Criteria, WM_SETFONT, WPARAM(hFont), TRUE);
		
			//Thêm các tiêu chí vào combobox
				AddCriteriaComboboxItem(h_cbb_Criteria);

		//Nội dung tiêu chí
		h_lab_Content = CreateWindowEx(0, L"STATIC", L"Nội dung",
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
			160, 50, 250, 20, hWnd, NULL, hInst, NULL);
		SendMessage(h_lab_Content, WM_SETFONT, WPARAM(hFont), TRUE);

		h_txt_Content = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER  |  BS_GROUPBOX ,
			160, 70, 250, 20, hWnd, (HMENU)IDC_TXT_CONTENT, hInst, NULL);
		SendMessage(h_txt_Content, WM_SETFONT, WPARAM(hFont), TRUE);	
			
		//Số tiền chi tiêu cho nội dung
		h_lab_Amount_Money = CreateWindowEx(0, L"STATIC", L"Số tiền",
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
			430, 50, 140, 20, hWnd, NULL, hInst, NULL);
		SendMessage(h_lab_Amount_Money, WM_SETFONT, WPARAM(hFont), TRUE);

		h_txt_Amount_Money = CreateWindowEx(0, L"EDIT", L"",
			WS_CHILD | WS_VISIBLE | WS_BORDER | BS_GROUPBOX | ES_NUMBER ,
			430, 70, 140, 20, hWnd, (HMENU)IDC_TXT_AMOUNT_MONEY, hInst, NULL);
		SendMessage(h_txt_Amount_Money, WM_SETFONT, WPARAM(hFont), TRUE);

		//Thêm
		h_btn_Add = CreateWindowEx(0, L"BUTTON", L"Thêm",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			460, 100, 100, 40, hWnd, (HMENU)IDC_BTN_ADD, hInst, NULL);
		SendMessage(h_btn_Add, WM_SETFONT, WPARAM(hFont), TRUE);
	//===========================
	//Group lịch sử chi tiêu
	h_grb_History = CreateWindowEx(0, L"BUTTON", L"Lịch sử chi tiêu",
		WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
		10, 150, 570, 180, hWnd, NULL, hInst, NULL);
	SendMessage(h_grb_History, WM_SETFONT, WPARAM(hFont), TRUE);
		//Nội dung lịch sử chi tiêu
		/*h_lb_History = CreateWindowEx(WS_EX_CLIENTEDGE, TEXT("listbox"), L"",
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL,
			40, 180, 500, 120, hWnd, (HMENU)IDC_LB_HISTORY, NULL, NULL);
		SendMessage(h_lb_History, WM_SETFONT, WPARAM(hFont), TRUE);*/

		h_lv_History = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEWW, L"",
			WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_EDITLABELS,
			40, 180, 500, 120, hWnd, (HMENU)IDC_LV_HISTORY, hInst, NULL);
		SendMessage(h_lv_History, WM_SETFONT, WPARAM(hFont), TRUE);
		
		//Tổng chi tiêu
		h_lab_Total_Money = CreateWindowEx(0, L"STATIC", L"Tổng chi tiêu:",
			WS_CHILD | WS_VISIBLE | WS_OVERLAPPED ,
			320, 300, 100, 20, hWnd, NULL, hInst, NULL);
		SendMessage(h_lab_Total_Money, WM_SETFONT, WPARAM(hFont), TRUE);

		h_txt_Total_Money = CreateWindowEx(0, L"EDIT", L"Total",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY | BS_GROUPBOX,
			410, 300, 130, 20, hWnd, (HMENU)IDC_TXT_TOTAL_MONEY, hInst, NULL);
		SendMessage(h_txt_Total_Money, WM_SETFONT, WPARAM(hFont), TRUE);

	//===========================
	//Thống kê
		
	h_lab_Statistical = CreateWindowEx(0, L"STATIC", L"Thống kê",
		WS_CHILD | WS_VISIBLE | WS_OVERLAPPED,
		20, 340, 80, 20, hWnd, NULL, hInst, NULL);
	SendMessage(h_lab_Statistical, WM_SETFONT, WPARAM(hFont), TRUE);
	

		WCHAR tembuf[50]; wsprintf(tembuf, L"%d", g_TotalMoney);
		SetWindowText(h_txt_Total_Money, tembuf);
	//===========================	
		InitListViewColumns(h_lv_History);
		g_chartStatistical.InitChartNote(hWnd, hInst);
}
//Click button Thêm
void OnclickButtonAdd()
{
	HWND hWnd = GetForegroundWindow();

	WCHAR * buff_Criteria = NULL;
	WCHAR * buff_Content = NULL;
	WCHAR * buff_AmountofMoney = NULL;
	WCHAR * buff_Sum = NULL;
	int size1, size2, size3, size4;

	//Đọc dữ liệu nhập
	size1 = GetWindowTextLength(h_cbb_Criteria) + 1;
	buff_Criteria = new WCHAR[size1];
	GetDlgItemText(hWnd, IDC_CBB_CRITERIA, buff_Criteria, size1);


	size2 = GetWindowTextLength(h_txt_Content) + 1;
	buff_Content = new WCHAR[size2];
	GetWindowText(h_txt_Content, buff_Content, size2);

	size3 = GetWindowTextLength(h_txt_Amount_Money) + 1;
	buff_AmountofMoney = new WCHAR[size3];
	GetWindowText(h_txt_Amount_Money, buff_AmountofMoney, size3);

	size4 = size1 + size2 + size3 + 6;

	//Kiểm tra dữ liệu nhập
	if (1 == size2)
	{
		MessageBox(0, L"Ô này không thể để trống. Hãy nhập vào một nội dung.", 0, 0);
		return;
	}

	if (1 == size3)
	{
		MessageBox(0, L"Ô này không thể để trống.Hãy nhập vào số tiền.", 0, 0);
		return;
	}

	//Phân loại số tiền theo các tiêu chí
	int cur_item = SendMessage(h_cbb_Criteria, CB_GETCURSEL, 0, 0);
	int money = _wtoi(buff_AmountofMoney);
	Arr_Money[cur_item] += money;

	//Thêm dữ liệu vào listview lịch sử
	g_Expense.set_Allvalue(cur_item, buff_Content, money);
	AddValueToListView(h_lv_History, g_Expense);
	//Lưu dữ liệu thay đổi vào file
	SavetoHistory(g_Expense);

	//Thay đổi biểu đồ	
	SendMessage(hWnd, WM_PAINT, 0, 0);
	InvalidateRect(hWnd, NULL, TRUE);
	UpdateWindow(hWnd);


	//Giải phóng
	delete[]buff_Criteria; buff_Criteria = NULL;
	delete[]buff_Content; buff_Content = NULL;
	delete[]buff_AmountofMoney; buff_AmountofMoney = NULL;
	delete[]buff_Sum;  buff_Sum = NULL;
}

//----------------------------------COMBOBOX-------------------
//Thêm các tiêu chí vào comboboxContent
void AddCriteriaComboboxItem(HWND h_cb_hWnd)
{
	WCHAR A[max_lenght_text];
	memset(&A, 0, sizeof(A));
	for (int i = 0; i < count_criteria; i++)
	{
		wcscpy_s(A, sizeof(A) / sizeof(WCHAR), (WCHAR*)Criterias[i]);
		// Thêm các tiêu chí vào combobox.
		SendMessageW(h_cb_hWnd, (UINT)CB_ADDSTRING, 0, (LPARAM)A);
	}
	//Chọn tiêu chí mặc định 
	SendMessageW(h_cb_hWnd, CB_SETCURSEL, (WPARAM)0, NULL);	
}

//----------------------------------LISTVIEW-------------------
//Nguồn tham khảo code MSDN
BOOL InitListViewColumns(HWND hWndListView)
{
	WCHAR szText[256];     // Temporary buffer.
	LVCOLUMN lvc;
	int iCol;
	const int C_COLUMNS = 3;

	// Initialize the LVCOLUMN structure.
	// The mask specifies that the format, width, text,
	// and subitem members of the structure are valid.
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	wsprintf(szText, L"Khoa");
	
	// Add the columns.
	//SubItem 0
	iCol = 0;	
	lvc.iSubItem = iCol;
	lvc.pszText = szText;
	lvc.cx = 90;               // Width of column in pixels.
	lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
	//Load the names of the column headings from the string resources.
	LoadStringW(hInst,
			IDS_FIRSTCOLUMN +1 + iCol,
			szText,
			sizeof(szText) / sizeof(szText[0] ));		
	// Insert the columns into the list view.
		if (ListView_InsertColumn(hWndListView, iCol, &lvc) == -1)
			return FALSE;

	//SubItem 1
		iCol++;		
		lvc.iSubItem = iCol;
		lvc.pszText = szText;
		lvc.cx = 255;               // Width of column in pixels.
		lvc.fmt = LVCFMT_CENTER;  // Left-aligned column.
		//Load the names of the column headings from the string resources.
		LoadStringW(hInst,
				IDS_FIRSTCOLUMN +1 + iCol,
				szText,
				sizeof(szText) / sizeof(szText[0] ));		
		// Insert the columns into the list view.
			if (ListView_InsertColumn(hWndListView, iCol, &lvc) == -1)
				return FALSE;

	//SubItem 2
		iCol++;	
		lvc.iSubItem = iCol;
		lvc.pszText = szText;
		lvc.cx = 130;               // Width of column in pixels.
		lvc.fmt = LVCFMT_RIGHT;  // Left-aligned column.
		//Load the names of the column headings from the string resources.
		LoadStringW(hInst,
				IDS_FIRSTCOLUMN +1 + iCol,
				szText,
				sizeof(szText) / sizeof(szText[0] ));		
		// Insert the columns into the list view.
			if (ListView_InsertColumn(hWndListView, iCol, &lvc) == -1)
				return FALSE;			
			
	return TRUE;
}
BOOL AddValueToListView(HWND hWndListView, CDailyExpense Expense){
	
	WCHAR buffer[30];
	LVITEM lvI;
	lvI.pszText = LPSTR_TEXTCALLBACK; // Sends an LVN_GETDISPINFO message.
	lvI.mask = LVIF_TEXT | LVIF_STATE;
	lvI.stateMask = 0;
		
	lvI.iSubItem = 0;
	lvI.state = 0;
	lvI.iItem = 0;
	lvI.pszText = getCriteriaByID(Expense.get_Criteria());
	if ( ListView_InsertItem(hWndListView, &lvI) == -1) { FALSE; }


	lvI.mask = LVIF_TEXT;
	//Cột đầu tiên là Nội dung
	lvI.iSubItem = 1;
	lvI.pszText =Expense.get_Content();
	ListView_SetItem(hWndListView, &lvI);
	//Cột thứ 3 là số tiền
	lvI.iSubItem = 2;
	wsprintf(buffer, L"%d", Expense.get_AmountOfMoney());
	lvI.pszText = buffer;
	ListView_SetItem(hWndListView, &lvI);

	g_TotalMoney += Expense.get_AmountOfMoney();
	WCHAR total_buff[50]; wsprintf(total_buff, L"%d", g_TotalMoney);
	SetWindowText(h_txt_Total_Money, total_buff);
	
	return TRUE;
}


//----------------------------------FILE-------------------
//Đọc nội dung lịch sử từ file lên
void LoadHistory()
{
	const int BLOCK_SIZE = 10000; //Số bytes mỗi lần đọc file
								//Doc file
	HANDLE hFile, hFiletempt;
	DWORD dwBytesRead, dwBytesWritten, dwPos;
	WCHAR buffer[BLOCK_SIZE + 2];
	WCHAR buffer2[BLOCK_SIZE + 2];
	CDailyExpense Expense;
	
	/* Mở tập tin có sẵn */
	hFile = CreateFile(_T(HISTORY_FILE_PATH), // tên tập tin
		GENERIC_READ, // mở để đọc
		0, // không chia sẻ
		NULL, // không bảo mật
		OPEN_EXISTING, // tập tin đã có sẵn
		FILE_ATTRIBUTE_NORMAL, // tập tin thông thường
		NULL); // không thuộc tính tạm
	wsprintf(buffer, L"");

	if (hFile == INVALID_HANDLE_VALUE)
	{
		//MessageBox(0, L"Could not open file.", L"Error", MB_OK);
		//Tạo mới file nếu chưa tồn tại
		hFiletempt = CreateFile(_T(HISTORY_FILE_PATH),
			GENERIC_WRITE,          // open for writing															  
			0,       // share for writing															  
			NULL,                  // default security															  
								   //  CREATE_NEW,         // existing file only															  
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL, // normal file															  
			NULL);                 // no attr. template		
		CloseHandle(hFiletempt);
	}
	else
	{

		wsprintf(buffer, L"");
		char c;

		ReadFile(hFile, buffer, BLOCK_SIZE, &dwBytesRead, NULL);
		if (buffer[0] == 0)
		{
			//MessageBox(0, L"file buffer load len =0", 0, 0);
		}
		else {
			buffer2[20000];
			int i = 0;
			int j = 0;
			int isRightExpense = 1;

				while (buffer[i] != NULL)
				{
					int CritialID = 0;
					int Money = 0;
					
					if (buffer[i] == '-')
					{
						buffer2[j] = '\0';
						j = 0;						
						switch (isRightExpense)
						{							
						case 1:							
							CritialID = _wtoi(buffer2);
							Expense.set_Criteria(CritialID);
							isRightExpense++;							
							break;
						case 2:
							Expense.set_Content(buffer2); isRightExpense++;							
							break;
						case 3:						
							Money = _wtoi(buffer2);
							Expense.set_AmountOfMoney(Money);
							isRightExpense = 1;
							AddValueToListView(h_lv_History, Expense); //Đưa thông tin vào listview			
							Arr_Money[Expense.get_Criteria()] += Money;							
							break;							
						}						
					}
					else
					{
						buffer2[j] = buffer[i];
						j++;
					}
					i++;
				}		
		}
		CloseHandle(hFile);

		HWND hWnd = GetForegroundWindow();
		SendMessage(hWnd, WM_PAINT, 0, 0);
		InvalidateRect(hWnd, NULL, TRUE);
		UpdateWindow(hWnd);
	}
}
void SavetoHistory(CDailyExpense Expense)
{
	HANDLE hFileOut;
	WCHAR buffer_file[512];

	wsprintf(buffer_file, L"%d-%s-%d-", Expense.get_Criteria(), Expense.get_Content(), Expense.get_AmountOfMoney());
	hFileOut = CreateFile(_T(HISTORY_FILE_PATH),               // file to open
		GENERIC_WRITE,          // open for writing
		0,       // share for writing
		NULL,                  // default security
							   //  CREATE_NEW,         // existing file only
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, // normal file
		NULL);                 // no attr. template

							   // Write to File
	BOOL bErrorFlag = FALSE;
	DWORD dwPtr = SetFilePointer(hFileOut, 0, NULL, FILE_END); //set pointer position to end file
	DWORD dwBytesToWrite = lstrlenW((LPCWSTR)buffer_file) * 2;
	bErrorFlag = WriteFile(
		hFileOut,           // open file handle
		buffer_file,      // start of data to write
		dwBytesToWrite,  // number of bytes to write
		&dwPtr, // number of bytes that were written
		NULL);            // no overlapped structure
	CloseHandle(hFileOut);
}

LPTSTR getCriteriaByID(int ID)
{
	switch (ID)
	{
	case 0:
		return L"Ăn uống";
		break;
	case 1:
		return L"Đi lại";
		break;
	case 2:
		return L"Nhà cửa";
		break;
	case 3:
		return L"Xe cộ";
		break;
	case 4:
		return L"Nhu yếu phẩm";
		break;
	case 5:
		return L"Dịch vụ";
		break;

	}
}