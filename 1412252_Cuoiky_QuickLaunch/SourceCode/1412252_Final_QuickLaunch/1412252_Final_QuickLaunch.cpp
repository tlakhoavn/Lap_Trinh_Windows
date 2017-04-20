// 1412252_Final_QuickLaunch.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1412252_Final_QuickLaunch.h"
#include <windowsx.h>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include <shellapi.h>

#include <winuser.h>
#include <commctrl.h>

#include <shobjidl.h>

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")

//Dùng để sử dụng hàm StrCpy, StrNCat
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")



#define BUFFER_LEN 260
#define KEY_LEN 260

#define TCHARPTR_TO_STRINGREF(ch)    gcnew System::String(p)
#define MAX_SUGGEST_ITEMS_VISIBLE 0 //Số item hiện trong danh sách gợi ý
#define BTN_HIDE_STATISTICS   2306

using namespace std;

typedef struct KEYWORD;			//Từ khóa tắt khi tìm kiếm
typedef struct FULLKEY;			//Từ khóa đầy đủ tên một tập tin exe trong bảng mapping
typedef struct REALLINK;		//Đường dẫn chứa tập tin exe

//Từ khóa là một đoạn con của tên file dùng để tìm kiếm
struct KEYWORD
{
	TCHAR m_Key[KEY_LEN];
	vector <FULLKEY *> m_List_FullKey;

	KEYWORD()
	{
	}
};

//Tên đầy đủ của một file, không bao gồm đuôi (mặc định đuôi là .exe rồi)
struct FULLKEY
{
	TCHAR m_FullKey[KEY_LEN];
	int m_Freq;           //Số lần gọi đến từ khóa này
	REALLINK * pReadLink;

	FULLKEY()
	{
		pReadLink = NULL;
		m_Freq = 1;
	}
};

//Đường dẫn đầy đủ đến một file 
struct REALLINK
{
	TCHAR m_ReadLink[BUFFER_LEN];
};



TCHAR buffer[BUFFER_LEN];

//@ Đường dẫn mặc định để load data nếu chưa có file cache lưu trữ trước đó.
TCHAR g_Directory_ProGramFiles[] = _T("C:\\Program Files");
TCHAR g_Directory_ProGramFiles_86[] = _T("C:\\Program Files (x86)");

vector <REALLINK *> g_List_ExeFileLink;          //Danh sách các đường dẫn đến các tập tin exe
vector <FULLKEY *> g_List_FullKey;        //Danh sách lưu tên các file exe được load lên và tầng số sử dụng
vector <KEYWORD *> g_List_KeyWord;        //Danh mục các từ khóa khi tìm kiếm


#define MAX_LOADSTRING 100
#define WM_USER_SHELLICON WM_USER +1


void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnPaint(HWND hwnd);
void OnDestroy(HWND hwnd);
BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);

size_t StrLen(PCWSTR pstr);    //Độ dài chuỗi WCHAR *
void LoadDataToMainMemory(); //Đọc và khởi tạo bảng mapping khi khởi động chương trình
bool ReadDataFromCache();       //Đọc dữ liệu bảng mapping từ cache, trả về true nếu cache tồn tự, false nếu không
bool ReadDataFromDrive(TCHAR * Directory);      //Tải lại dữ liệu từ thư mục gốc nếu chưa lưu cache hoặc có yêu cầu từ người dùng, return true nếu thành công
void SaveDataToCache();         //Lưu dữ liệu đọc vào cache
HINSTANCE _stdcall OpenExecuteFile(HWND hWnd, LPCWSTR IpFile,
	LPCWSTR IpOperator = _T("open"), LPCWSTR IpParameters = NULL, LPCWSTR IpDirectory = NULL, INT nShowCmd = SW_NORMAL); //Chạy file exe
bool isExecuteFile(WCHAR * FileName);          //Kiểm tra một file có phải là file thực thi .exe
void AddKeyWordToLibrary(PWSTR psKeyWord, PWSTR psRealLink, int Freq = 1);   //Thêm một từ khóa và đường dẫn đến file exe vào bảng mapping
int FindPosOfKeyWordInList(PWSTR psKeyWord, vector<KEYWORD *> vtListKey); //Tìm kiếm vị trí của từ khóa trong mảng
int FindPosOfFullKeyInList(PWSTR psKeyWord,PWSTR psReadLink, vector<FULLKEY *> vtListKey); //Tìm kiếm vị trí của một tên file trong mảng tên file
void AddKeyWordToList(KEYWORD* pkwKeyWord, vector<KEYWORD *> &vtListKey);      //Thêm từ khóa vào bảng là một mãng đã sắp xếp

void Show_Suggestion_ListKey(LPCWSTR Key);                       //Tải lại bảng gợi ý tìm kiếm
void Increase_frequency_and_Short(vector<FULLKEY *>, int positionSel); //Tăng tần số từ khóa và sắp xếp lại bảng từ khóa có chứa khóa vừa tìm
void SortListPointToFullKey(vector <FULLKEY *> &List_FullKey);          //Sắp xếp mảng con trỏ đến từ khóa đầy đủ, dùng để sắp xếp mức độ ưu tiên theo tần số khi hiển thị
void ShowStatistics(HWND hWnd_);                                                  //Hiển thị biểu đồ thống kê tần số sử dụng
void HideStatistics(HWND hWnd_);                                                  //Ẩn biểu đồ thống kê tần số
void DrawChart(HDC hdc_);                                                         //Vẽ biểu đồ
void LoadDataByOpenDialog(HWND hWnd_);                                                            //Hiển thị hộp thoại mở file

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

NOTIFYICONDATA nidApp;
HMENU hPopMenu;

HWND hWndComboBox, hwndCombo;
HWND btnHideStatistics;
//HWND txtTopKey_1, txtTopKey_2, txtTopKey_3, txtTopKey_4;
HWND sttext_Chart_Key[5];     //Các text hiển thị tên các từ khóa trong biểu đồ thống kê
HWND sttext_Chart_Seq[5];     //Các text hiển thị tần số các từ khóa trong biểu đồ thống kê
BOOL StatisticsChange;        //Đánh dấu khi biểu đồ thay đổi.

WNDPROC lpfnComboboxWndProc;
WNDPROC lpfnInputTextWndProc;
HINSTANCE hDLLKeyBoardHook;

//Kích thước khung tìm kiếm
#define xpos  0
#define ypos  3
#define nwidth  320
#define nheight  22
//Kích thước cửa sổ ứng dụng
#define WIN_POSTION_X 300
#define WIN_POSTION_Y 300
#define WIN_SIZE_WIDTH 335
#define WIN_SIZE_HEIGHT 90
#define WIN_SIZE_HEIGHT_EXTRA 300 //Kích chiều cao mở rộng để hiển thị biểu đồ

#define CACHE_FILE_NAME "cache.txt"          //Tên file cache lưu bảng dữ liệu

clock_t g_Time_Show_Suggest;         //Thời điểm hiển thị gợi ý
clock_t g_Time_Show_Suggest_Next;    //Thời điểm hiển thị gợi ý kế tiếp
#define TIME_TO_SHOW_SUGGEST 1000     //Thời gian giản cách tối thiểu khi gõ, để bắt phím và hiển thị bảng gợi ý mới
bool g_Found_result;                //Đánh dấu có tìm kiếm từ khóa hay không

KEYWORD * Found_KeyWord;              //Con trỏ đến từ khóa đang được tìm kiếm

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	ComboboxProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

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
    LoadStringW(hInstance, IDC_MY1412252_FINAL_QUICKLAUNCH, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1412252_FINAL_QUICKLAUNCH));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_QUICKLAUNCH));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1412252_FINAL_QUICKLAUNCH);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_QUICKLAUNCH));

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
	HWND hWnd;
	HICON hMainIcon;

	hInst = hInstance; // Store instance handle in our global variable

    hWnd = CreateWindowW(szWindowClass, szTitle,
	    WS_SYSMENU | WS_CAPTION | WS_BORDER | WS_OVERLAPPED | WS_VISIBLE | WS_CLIPCHILDREN | WS_MINIMIZEBOX,
	   WIN_POSTION_X, WIN_POSTION_Y, WIN_SIZE_WIDTH, WIN_SIZE_HEIGHT, nullptr, nullptr, hInstance, nullptr);

	hMainIcon = LoadIcon(hInstance, (LPCTSTR)MAKEINTRESOURCE(IDI_QUICKLAUNCH));

	nidApp.cbSize = sizeof(NOTIFYICONDATA); // sizeof the struct in bytes 
	nidApp.hWnd = (HWND)hWnd;              //handle of the window which will process this app. messages 
	nidApp.uID = IDI_QUICKLAUNCH;           //ID of the icon that willl appear in the system tray 
	nidApp.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP; //ORing of all the flags 
	nidApp.hIcon = hMainIcon; // handle of the Icon to be displayed, obtained from LoadIcon 
	nidApp.uCallbackMessage = WM_USER_SHELLICON;
	LoadString(hInstance, IDS_APP_TITLE, nidApp.szTip, MAX_LOADSTRING);
	Shell_NotifyIcon(NIM_ADD, &nidApp);

   if (!hWnd)
   {
      return FALSE;
   }  

   StatisticsChange = TRUE;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   //ShowWindow(hWnd, SW_HIDE);
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
	int wmID, wmEvent;
	POINT lpClickPoint;

    switch (message)
    {
	case WM_USER_SHELLICON:
		// systray msg callback 
		switch (LOWORD(lParam))
		{
		case WM_RBUTTONDOWN:
			UINT uFlag = MF_BYPOSITION | MF_STRING;
			GetCursorPos(&lpClickPoint);
			hPopMenu = CreatePopupMenu();
			
			InsertMenu(hPopMenu, 0xFFFFFFFF, uFlag, IDM_ABOUT, _T("About"));
			InsertMenu(hPopMenu, 0xFFFFFFFF, MF_SEPARATOR, 0, _T(""));
			InsertMenu(hPopMenu, 0xFFFFFFFF, uFlag, IDM_STATISTICS, _T("Statistics"));

			if (IsWindowVisible(hWnd) == true)
			{
				InsertMenu(hPopMenu, 0xFFFFFFFF, MF_GRAYED, IDM_SHOW, _T("Show"));
				InsertMenu(hPopMenu, 0xFFFFFFFF, uFlag, IDM_HIDE, _T("Hide"));
			}
			else
			{
				InsertMenu(hPopMenu, 0xFFFFFFFF, uFlag, IDM_SHOW, _T("Show"));
				InsertMenu(hPopMenu, 0xFFFFFFFF, MF_GRAYED, IDM_HIDE, _T("Hide"));
			}
			
			InsertMenu(hPopMenu, 0xFFFFFFFF, MF_SEPARATOR, 0, _T(""));
			InsertMenu(hPopMenu, 0xFFFFFFFF, uFlag, IDM_EXIT, _T("Exit"));

			SetForegroundWindow(hWnd);
			TrackPopupMenu(hPopMenu, TPM_LEFTBUTTON | TPM_LEFTALIGN |  TPM_RIGHTBUTTON | TPM_BOTTOMALIGN, lpClickPoint.x, lpClickPoint.y, 0, hWnd, NULL);
			return TRUE;

		}
		break;

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
//Subclass Proc của edittext con của combobox
LRESULT CALLBACK ComboboxProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int CurSel;
	switch (msg)
	{
	case WM_KEYUP:
		switch (wParam)
		{		
		case VK_UP:
		case VK_DOWN:
		case VK_LEFT:
		case VK_RIGHT:
			if (g_Found_result == FALSE)    //Combobox chứa dòng thông báo thay vì một giá trị tìm thấy thì chặn nhận giá trị
			{
				SetWindowText(hwndCombo, L"");
			}
			break;
		case VK_TAB:                        //Người dùng nhấn tab hoặc enter để mở file
		case VK_RETURN:
			CurSel = SendMessage(hWndComboBox, CB_GETCURSEL, 0, 0);
			if (Found_KeyWord != NULL)
			{				
				OpenExecuteFile(hwnd, Found_KeyWord->m_List_FullKey[CurSel]->pReadLink->m_ReadLink);
				//MessageBox(0, Found_KeyWord->m_List_FullKey[CurSel]->pReadLink->m_ReadLink, L"Mở file", 0);
				Increase_frequency_and_Short(Found_KeyWord->m_List_FullKey, CurSel); //Tăng tầng số sử dụng từ khóa
			}
			
			break;
		default:
			
			GetWindowText(hwnd, buffer, 255);
			
			g_Time_Show_Suggest_Next = clock();
			if (g_Time_Show_Suggest_Next - g_Time_Show_Suggest > TIME_TO_SHOW_SUGGEST)
			{
				if (StrCmp(buffer, L" ") == 0)  //Loại khoảng trắng đầu từ khóa
				{
					SetWindowText(hwndCombo, L"");
				}
				else
				{
					Show_Suggestion_ListKey(buffer);
					g_Time_Show_Suggest = g_Time_Show_Suggest_Next;
				}
				
			}
					
						
		}
		break;
	}
	return CallWindowProc(lpfnComboboxWndProc, hwnd, msg, wParam, lParam);
}

BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	g_Time_Show_Suggest = clock(); //Khởi động mốc tời điểm tính khoảng cách tối thiểu giữa 2 lần nhập ký tự để hiển thị gợi ý
	g_Found_result = FALSE;        //Chưa có từ khóa, chưa có gợi ý
	Found_KeyWord = NULL;

	//Init hook dll to hook window keyboard	
	hDLLKeyBoardHook = LoadLibrary(_T("KeyBoardHook.dll"));
	typedef int(*procHook) (HWND);
	if (hDLLKeyBoardHook != NULL)
	{
		procHook installHookproc = (procHook)GetProcAddress(hDLLKeyBoardHook, "InitKeyBoardHook");
		if (installHookproc != NULL)
		{
			int rs = installHookproc(hWnd);
			if (rs == 1)
			{
				//MessageBox(hWnd, _T("Install hook success"), _T("Info"), MB_OK);
			}
			else
			{
				MessageBox(hWnd, _T("Install hook failed"), _T("Error"), MB_OK);
			}
		}
		else
		{
			MessageBox(hWnd, _T("Get proc failed"), _T("Error"), MB_OK);
		}
	}
	else
	{
		MessageBox(hWnd, _T("Load DLL failed"), _T("Error"), MB_OK);
	}


	// Get system font
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);	

	//Button ẩn khu vực biểu đồ
	btnHideStatistics = CreateWindowEx(0, WC_BUTTON, TEXT("Hide"),
		WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_BORDER,
		nwidth - 60,ypos + 30 , 50, nheight, hWnd, (HMENU)BTN_HIDE_STATISTICS, hInst,
		NULL);
	SendMessage(btnHideStatistics, WM_SETFONT, WPARAM(hFont), TRUE);
	//Khởi tạo các static text hiển thị cho biểu đồ
	for (int i = 0; i < 5; i++)
	{
		sttext_Chart_Key[i] = CreateWindowEx(0, WC_STATIC, TEXT(""),
			WS_CHILD | WS_OVERLAPPED | WS_VISIBLE ,
			0,0,0,0, hWnd, NULL, hInst,	NULL);
		SendMessage(sttext_Chart_Key[i], WM_SETFONT, WPARAM(hFont), TRUE);

		sttext_Chart_Seq[i] = CreateWindowEx(0, WC_STATIC, TEXT(""),
			WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
			0, 0, 0, 0, hWnd, NULL, hInst, NULL);
		SendMessage(sttext_Chart_Seq[i], WM_SETFONT, WPARAM(hFont), TRUE);
	}

	hWndComboBox = CreateWindowEx(0,WC_COMBOBOX, TEXT(""),
		CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | SS_LEFT | WS_BORDER,
		xpos, ypos, nwidth, nheight, hWnd, NULL, hInst,
		NULL);
	SendMessage(hWndComboBox, WM_SETFONT, WPARAM(hFont), TRUE);	

	//Get hwnd of edit control in combobox created earlier.
	hwndCombo = GetWindow(hWndComboBox, GW_CHILD);
	//Use SetWindowLong to create subclass, lpfnComboboxWndProc is original proc
	lpfnComboboxWndProc = (WNDPROC)SetWindowLong(hwndCombo, GWL_WNDPROC, (DWORD)ComboboxProc);	
			
	//Đọc dữ liệu vào bộ nhớ chính
	LoadDataToMainMemory();	
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
	case IDM_SHOW:
		ShowWindow(hWnd, SW_RESTORE);
		break;
	case IDM_HIDE:
		ShowWindow(hWnd, SW_HIDE);
		break;
	case IDM_LOAD:
		LoadDataByOpenDialog(hWnd);
		break;
	case IDM_STATISTICS:
		ShowStatistics(hWnd); //Hiện biểu đồ thống kê tần số sử dụng.
		break;
	case BTN_HIDE_STATISTICS:
		HideStatistics(hWnd);
		break;		
	}
}
void OnPaint(HWND hWnd)
{
	SetFocus(hwndCombo); //Đưa con trỏ chính vào edit của combobox
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hWnd, &ps);
	// TODO: Add any drawing code here...
	DrawChart(hdc);	
	EndPaint(hWnd, &ps);
}
void OnDestroy(HWND hwnd)
{
	if (hDLLKeyBoardHook != NULL)
	{
		typedef void(*proc)();
		proc uninstallHook = (proc)GetProcAddress(hDLLKeyBoardHook, "UninstallKeyBoardHook");
		if (uninstallHook != NULL)
		{
			uninstallHook();
		}
		FreeLibrary(hDLLKeyBoardHook);
	}

	SaveDataToCache();
	PostQuitMessage(0);
}

//Đọc và khởi tạo bảng mapping khi khởi động chương trình
void LoadDataToMainMemory() 
{
	if (ReadDataFromCache() == false)
	{
		ReadDataFromDrive(g_Directory_ProGramFiles);
		ReadDataFromDrive(g_Directory_ProGramFiles_86);
		//SaveDataToCache();
		MessageBox(0, L"Read data from the drive success", L"Load success", 0);
	}
	else
	{
		//SaveDataToCache();
	}

}
//Đọc dữ liệu bảng mapping từ cache, trả về true nếu cache tồn tự, false nếu không
bool ReadDataFromCache()       
{
	//return false;
	fstream fi;

	char buff_key[BUFFER_LEN];
	char buff_link[BUFFER_LEN];

	TCHAR FullKey_[BUFFER_LEN];
	TCHAR Link_[BUFFER_LEN];

	int Freq_use_Key = 0;


	fi.open(CACHE_FILE_NAME, std::ios::_Nocreate);	
	
	
	if (!fi)
	{
		return false;
	}
	else
	{
		while (!fi.eof())
		{			
			fi.getline(buff_key, BUFFER_LEN);
			fi.getline(buff_link, BUFFER_LEN);

			fi >> Freq_use_Key;		fi.get();// Đọc tần số và xuống dòng
			wsprintf(FullKey_, L"%hs", buff_key); 
			wsprintf(Link_, L"%hs", buff_link);
			try
			{				
				AddKeyWordToLibrary(FullKey_, Link_, Freq_use_Key);
			}
			catch (exception e)
			{

			}			
		}				
			
	}
	
	
	fi.close();
	return true;
}
//Tải lại dữ liệu từ thư mục gốc nếu chưa lưu cache hoặc có yêu cầu từ người dùng
bool ReadDataFromDrive(TCHAR * Directory)      
{
	StrCpyW(buffer, Directory);

	vector<TCHAR*> Queue_Folder;
	TCHAR * queue_item;
	TCHAR * RealLink;                 //Lưu đường link đến file thực thi

	queue_item = new TCHAR[BUFFER_LEN];
	StrCpy(queue_item, buffer);
	Queue_Folder.push_back(queue_item);

	WIN32_FIND_DATA fd;
	HANDLE hFile;
	BOOL bFound;

	while (Queue_Folder.size() != 0)  //Khử đệ quy duyệt thư mục
	{

		bFound = true;

		TCHAR temp_buffer[BUFFER_LEN];                //Đường dẫn thư mục đang xét
		TCHAR temp_buffer_search[BUFFER_LEN];         //Đường dẫn thư mục đang xét có chưa ký tự * ở cuối chuỗi
		TCHAR Full_Key[BUFFER_LEN];                   

		StrCpyW(temp_buffer, Queue_Folder.back());

		delete Queue_Folder.back();					 //Giải phóng bộ nhớ       
		Queue_Folder.pop_back();


		StrCat(temp_buffer, _T("\\"));
		wsprintf(temp_buffer_search, _T("%s"), temp_buffer);
		StrCat(temp_buffer_search, _T("*"));

		hFile = FindFirstFileW(temp_buffer_search, &fd);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			bFound = false;
		}
		else
		{
			bFound = true;
		}


		//Trong khi còn tìm thấy file hoặc thư mục thư mục trong đường dẫn đang xét
		while (bFound)
		{
			if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				&& (StrCmp(fd.cFileName, _T(".")) != 0) && (StrCmp(fd.cFileName, _T("..")) != 0))
			{

				bFound = FALSE;
				queue_item = new TCHAR[BUFFER_LEN];
				StrCpy(queue_item, temp_buffer);
				StrCat(queue_item, fd.cFileName);
				Queue_Folder.push_back(queue_item);

			}
			else if (fd.dwFileAttributes& FILE_ATTRIBUTE_ARCHIVE)
			{
				if (isExecuteFile(fd.cFileName) == true)
				{
					//Thưc hiện việc add thêm vào bộ nhớ ở đây
					RealLink = new TCHAR[BUFFER_LEN];
					StrCpy(RealLink, temp_buffer);
					
					StrCpyN(Full_Key, fd.cFileName, StrLen(fd.cFileName) - 3);  
					StrCat(RealLink, fd.cFileName);
					
					AddKeyWordToLibrary(Full_Key, RealLink);	////////////////////////////////////////////////				
				}
			}

			bFound = FindNextFileW(hFile, &fd);
		}//While bFound		

	}//White Queue_Folder		

	return true;
}
//Lưu file cache cho những lần khởi động sau
void SaveDataToCache()
{
	fstream fo;
	fo.open(CACHE_FILE_NAME, std::ios::out);

	WCHAR Str_print[255] = { 0 };
	for (int i = 0; i < g_List_FullKey.size(); i++)
	{

		std::string strFullKey = std::string();
		std::string strLink = std::string();


		strFullKey.resize(WideCharToMultiByte(CP_UTF8, 0, g_List_FullKey[i]->m_FullKey, -1, NULL, 0, 0, 0));
		char* ptrKey = &strFullKey[0];
		WideCharToMultiByte(CP_UTF8, 0, g_List_FullKey[i]->m_FullKey, -1, ptrKey, strFullKey.size(), 0, 0);

		strLink.resize(WideCharToMultiByte(CP_UTF8, 0, g_List_FullKey[i]->pReadLink->m_ReadLink, -1, NULL, 0, 0, 0));
		char* ptrLink = &strLink[0];
		WideCharToMultiByte(CP_UTF8, 0, g_List_FullKey[i]->pReadLink->m_ReadLink, -1, ptrLink, strLink.size(), 0, 0);

		//MessageBox(0, g_List_FullKey[i]->pReadLink->m_ReadLink, 0, 0);

		fo << strFullKey << "\n" << strLink << "\n" <<g_List_FullKey[i]->m_Freq <<"\n";
	}
	fo.close();
}

//Thêm một từ khóa và đường dẫn đến file exe vào bảng mapping
void AddKeyWordToLibrary(PWSTR psKeyWord, PWSTR psRealLink, int Freq)   
{
	TCHAR * ptcFullKey = NULL;
	TCHAR * ptcKeyWord = NULL;

	KEYWORD * Key_Word = NULL;
	FULLKEY * Full_Key = new FULLKEY();
	REALLINK * Real_Link = new REALLINK();


	ptcFullKey = new TCHAR[KEY_LEN];
	StrCpy(ptcFullKey, psKeyWord);

	//Thêm một đường dẫn mới vào bảng danh mục các đường dẫn
	StrCpy(Real_Link->m_ReadLink, psRealLink);
	g_List_ExeFileLink.push_back(Real_Link);

	StrCpy(Full_Key->m_FullKey, ptcFullKey);
	Full_Key->pReadLink = Real_Link;       //Trỏ từ tên file đến đường dẫn
	Full_Key->m_Freq = Freq;
	g_List_FullKey.push_back(Full_Key);


	//Thêm từ khóa tìm kiếm vào bảng từ khóa tìm kiếm
	//Vòng for j là vị trí kí tự đầu chuỗi, tăng dầu
	//Vòng for i là độ dài chuỗi cắt ra với kí tự đầu chuỗi ở vị trí j
	//vd key'Chrome' => C Ch Chr Chro Chrom Chrome h hr hro hrom hrome r ....
	/*
	for (int j = 0; j < StrLen(ptcFullKey); j++)
	{
	for (int i = 1; i <= StrLen(ptcFullKey)-j; i++)
	{
	ptcKeyWord = new TCHAR[i + 2];
	StrCpyN(ptcKeyWord, ptcFullKey+j, i + 1);

	int Pos = FindPosOfKeyWordInList(ptcKeyWord, g_List_KeyWord);
	if (Pos >= 0) //Đã tồn tại từ khóa
	{
	g_List_KeyWord[Pos]->m_List_FullKey.push_back(Full_Key); //Thêm liên kết đến tên file mà từ khóa này có trỏ tới.

	}
	else //Từ khóa chưa tồn tại
	{
	Key_Word = new KEYWORD();
	StrCpy(Key_Word->m_Key, ptcKeyWord);
	Key_Word->m_List_FullKey.push_back(Full_Key); //Thêm liên kết tên file mà từ khóa này trỏ tới
	AddKeyWordToList(Key_Word, g_List_KeyWord);  //Thêm từ khóa vào bảng
	}

	}

	}*/

	for (int i = 1; i <= StrLen(ptcFullKey); i++)
	{
		ptcKeyWord = new TCHAR[i + 2];
		StrCpyN(ptcKeyWord, ptcFullKey, i + 1);

		int Pos = FindPosOfKeyWordInList(ptcKeyWord, g_List_KeyWord);
		if (Pos >= 0) //Đã tồn tại từ khóa
		{
			g_List_KeyWord[Pos]->m_List_FullKey.push_back(Full_Key); //Thêm liên kết đến tên file mà từ khóa này có trỏ tới.

		}
		else //Từ khóa chưa tồn tại
		{
			Key_Word = new KEYWORD();
			StrCpy(Key_Word->m_Key, ptcKeyWord);
			Key_Word->m_List_FullKey.push_back(Full_Key); //Thêm liên kết tên file mà từ khóa này trỏ tới	
						
			AddKeyWordToList(Key_Word, g_List_KeyWord);  //Thêm từ khóa vào bảng			
		}
	}

	if (ptcFullKey != NULL)
	{
		delete ptcFullKey;
	}

	if (ptcKeyWord != NULL)
	{
		delete ptcKeyWord;
	}


}
//Tìm kiếm vị trí của từ khóa trong mảng
int FindPosOfKeyWordInList(PWSTR psKeyWord, vector<KEYWORD *> vtListKey) 
{
	/*for (int pos = 0; pos < vtListKey.size(); pos++)
	{
	if (StrCmp(psKeyWord, vtListKey[pos]->m_Key) == 0)
	{
	return pos;
	}
	}*/

	if (vtListKey.size() == 0) return -1;

	int left = 0;
	int right = vtListKey.size() - 1;
	int mid;
	//Binary Search
	do
	{
		mid = (left + right) / 2;
		if (StrCmpI(psKeyWord, vtListKey[mid]->m_Key) == 0)
		{
			return mid;//Thấy x tại mid
		}
		else if (StrCmpI(psKeyWord, vtListKey[mid]->m_Key) <0)
			right = mid - 1;
		else
			left = mid + 1;
	} while (left <= right);

	return -1;// tìm hết dãy mà không có x
}
//Tìm kiếm vị trí của tên file trong bảng
int FindPosOfFullKeyInList(PWSTR psKeyWord, PWSTR RealLink, vector<FULLKEY *> vtListKey) 
{

	int result = -1;
	for (int i = 0; i < vtListKey.size(); i++)
		if (StrCmpI(vtListKey[i]->m_FullKey, psKeyWord) == 0 && StrCmpI(vtListKey[i]->pReadLink->m_ReadLink,RealLink)==0 ) return i;

			return result;
}
//Thêm từ khóa vào bảng là một mãng đã sắp xếp
void AddKeyWordToList(KEYWORD* pkwKeyWord, vector<KEYWORD *> &vtListKey)      
{

	if (vtListKey.size() == 0)
	{
		vtListKey.push_back(pkwKeyWord);
		return;
	}

	//int lenght = vtListKey.size();
	//for (int i = 0; i < lenght; i++)
	//{
	//	if (StrCmp(vtListKey[i]->m_Key, pkwKeyWord->m_Key) >0)       //Thêm vào mãng tăng dần đều
	//	{
	//		vtListKey.insert(vtListKey.begin() + i, pkwKeyWord);
	//			return;
	//	}
	//}
	//vtListKey.push_back(pkwKeyWord);

	int left = 0;
	int right = vtListKey.size() - 1;
	int mid;
	//Binary Search
	do
	{
		mid = (left + right) / 2;
		if (StrCmp(pkwKeyWord->m_Key, vtListKey[mid]->m_Key) <0)
			right = mid - 1;
		else
			left = mid + 1;
	} while (left <= right);

	vtListKey.insert(vtListKey.begin() + left, pkwKeyWord);
}
//Kiểm tra một file có phải là file thực thi .exe
bool isExecuteFile(WCHAR * FileName)          
{
	if (StrStrW(FileName, L".exe") == NULL)
	{
		return false;
	}
	else if (StrLen(StrStrW(FileName, L".exe")) != 4)
	{
		return false;
	}

	return true;
}
HINSTANCE _stdcall OpenExecuteFile(HWND hWnd, LPCWSTR IpFile, LPCWSTR IpOperator, LPCWSTR IpParameters, LPCWSTR IpDirectory, INT nShowCmd) //Chạy file exe
{
	//MessageBox(0, IpFile, 0, 0);
	return ShellExecuteW(hWnd, IpOperator, IpFile, IpParameters, IpDirectory, nShowCmd);
}


//Tải lại bảng gợi ý tìm kiếm
void Show_Suggestion_ListKey(LPCWSTR Key)                       
{
	WCHAR Key_[BUFFER_LEN];
	StrCpy(Key_, Key);

	//B1: Xóa list gợi ý cũ
	SendMessage(hWndComboBox, CB_RESETCONTENT, TRUE, 0);
	SendMessage(hWndComboBox, CB_SHOWDROPDOWN, FALSE, 0); //Đóng danh sách đang hiện
	//B2: Tìm kiếm từ khóa trong bảng danh mục
	int pos = FindPosOfKeyWordInList(Key_, g_List_KeyWord);
	//B3: Thêm danh sách gợi ý vào list
	
	
	if (pos >= 0)
	{
		SortListPointToFullKey(g_List_KeyWord[pos]->m_List_FullKey); //Sắp xếp thứ tự hiển thị gợi ý dựa vào tần số sử dụng

		g_Found_result = TRUE;
		Found_KeyWord = g_List_KeyWord[pos];
		int i;		
		for (i = 0; i < Found_KeyWord->m_List_FullKey.size(); i++)
		{
			SendMessage(hWndComboBox, CB_ADDSTRING, (WPARAM)0, (LPARAM)g_List_KeyWord[pos]->m_List_FullKey[i]->m_FullKey);
		}		
	}
	else
	{
		SendMessage(hWndComboBox, CB_ADDSTRING, (WPARAM)0, (LPARAM)_T("No results found for your search"));
		g_Found_result = FALSE;
		Found_KeyWord = NULL;
	}
	//B4: Hiện danh sách dropdown combobox	
	SendMessage(hWndComboBox, CB_SHOWDROPDOWN, TRUE, 0);
	SendMessage(hWndComboBox, CB_SETMINVISIBLE, MAX_SUGGEST_ITEMS_VISIBLE, 0);
	//B5: Trả lại từ khóa đang nhập cho combobox
	SetWindowText(hWndComboBox, Key_);	
	int pos_sel = StrLen(Key_); //Đưa con trỏ nhập vè cuối edit text
	SendMessage(hwndCombo, EM_SETSEL, (WPARAM)pos_sel, (LPARAM)pos_sel);
	

}
//Tăng tần số từ khóa và sắp xếp lại bảng từ khóa có chứa khóa vừa tìm
void Increase_frequency_and_Short(vector<FULLKEY *> ListFullKey, int positionSel)
{
	ListFullKey[positionSel]->m_Freq++; //Tăng tần số
	//==============Chưa sắp xếp được vì còn dính tới con trỏ
}
//Sắp xếp mảng con trỏ đến từ khóa đầy đủ, dùng để sắp xếp mức độ ưu tiên theo tần số khi hiển thị
void SortListPointToFullKey(vector <FULLKEY *> &List_FullKey)
{
	
	for (int i = List_FullKey.size() - 1; i>0;i--)
	{
		for (int j = 0; j < i; j++)
		{
			if (List_FullKey[j]->m_Freq < List_FullKey[i]->m_Freq) swap(List_FullKey[j], List_FullKey[i]);
		}
	}	
}
//Hiển thị biểu đồ thống kê tần số sử dụng
void ShowStatistics(HWND hWnd)
{
	RECT rect;
	GetWindowRect(hWnd, &rect);

	ShowWindow(hWnd, SW_RESTORE);
	SortListPointToFullKey(g_List_FullKey);
	SetWindowPos(hWnd, NULL, rect.left, rect.top, WIN_SIZE_WIDTH + 10, WIN_SIZE_HEIGHT_EXTRA, NULL);
	StatisticsChange = TRUE;
}
//Ẩn biểu đồ thống kê tần số
void HideStatistics(HWND hWnd)
{
	RECT rect;
	GetWindowRect(hWnd, &rect);
	SetWindowPos(hWnd, NULL, rect.left, rect.top, WIN_SIZE_WIDTH, WIN_SIZE_HEIGHT, NULL);
}
//Vẽ biểu đồ thống kê tần số
void DrawChart(HDC hdc)
{
	const int Chart_width = 30;
	const int Chart_bottom = 210;
	const int Chart_top = 70;
	const int Chart_left = 0.05 *WIN_SIZE_WIDTH;

	WCHAR buf_Seq[16];

	int Seq[4] = { 0 };
	int Chart_Height[5] = { 0 };
	
	Chart_Height[4] = 5;

	if (g_List_FullKey.size() >= 4)
	{
		for (int i = 0; i < 4; i++)
		{			
			Seq[i] = g_List_FullKey[i]->m_Freq;
			Chart_Height[i] = (int) (((float)Seq[i] / (float)Seq[0])*(Chart_bottom - Chart_top));
			
			wsprintf(buf_Seq, L"%d", Seq[i]);
			SetWindowText(sttext_Chart_Seq[i], buf_Seq);
			SetWindowText(sttext_Chart_Key[i], g_List_FullKey[i]->m_FullKey);
		}		
	}
	else return;

	wsprintf(buf_Seq, L"<%d", Seq[3]);
	SetWindowText(sttext_Chart_Seq[4], buf_Seq);
	SetWindowText(sttext_Chart_Key[4], L"other");

	HPEN hPen = CreatePen(PS_NULL, 1, RGB(0, 256, 64));
	HPEN holdPen = (HPEN)SelectObject(hdc, hPen);
	HBRUSH hbrush = CreateSolidBrush(RGB(0, 0, 80));
	HBRUSH holdBrush = (HBRUSH)SelectObject(hdc, hbrush);	
	
	for (int i = 0; i < 5; i++)
	{
		Rectangle(hdc, Chart_left + (int)(i*0.18*WIN_SIZE_WIDTH), Chart_bottom - Chart_Height[i], Chart_left +(int)(i*0.18*WIN_SIZE_WIDTH) + Chart_width, Chart_bottom);
	}

	if (StatisticsChange == TRUE)
	{
		for(int i=0;i<5;i++)
		{
		SetWindowPos(sttext_Chart_Key[i], NULL, Chart_left + (int)(i*0.18*WIN_SIZE_WIDTH), Chart_bottom, 2 * Chart_width, 20, NULL);
		SetWindowPos(sttext_Chart_Seq[i], NULL, Chart_left + (int)(i*0.18*WIN_SIZE_WIDTH) + 10, Chart_bottom - Chart_Height[i] - 20,Chart_width, 20, NULL);
		}
		StatisticsChange = FALSE;
	}

	
}

//Hiển thị hộp thoại mở file
void LoadDataByOpenDialog(HWND hWnd)
{
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
		COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileOpenDialog *pFileOpen;

		//Khởi tạo
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));

		if (SUCCEEDED(hr))
		{
			// Hiển thị hộp thoại
			hr = pFileOpen->Show(hWnd);

			// Đọc tên file ful name và reallink để xử lý thêm vào thư viện
			if (SUCCEEDED(hr))
			{
				IShellItem *pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					LPWSTR pszFilePath;
					LPWSTR pszFileName;

					hr = pItem->GetDisplayName(SIGDN_PARENTRELATIVEEDITING, &pszFilePath);
					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFileName);
					
					if (SUCCEEDED(hr))
					{
						if (isExecuteFile(pszFileName) == false)
						{
							MessageBox(hWnd, L"This is not an executable file", L"Load fail", MB_OK);
						}
						else
						{
							hr = pItem->GetDisplayName(SIGDN_NORMALDISPLAY, &pszFileName);
							int pos = FindPosOfFullKeyInList(pszFileName, pszFilePath, g_List_FullKey);
							if (pos >= 0)
							{								
									MessageBox(hWnd, L"File already exists", L"Load fail", MB_OK);							
									
							}
							else
							{
								AddKeyWordToLibrary(pszFileName, pszFilePath);
								MessageBox(hWnd, L"Successfully add new files", L"Success", MB_OK);
							}
														
						}
						CoTaskMemFree(pszFileName);
						CoTaskMemFree(pszFilePath);
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}

}

////////////////////////////////////////////////////////////////////////////////
size_t StrLen(PCWSTR pstr)
{
	if (pstr == NULL) return 0;
	int len = 0;
	while (pstr[len] != '\0')
	{
		len++;
	}
	return len;
}

