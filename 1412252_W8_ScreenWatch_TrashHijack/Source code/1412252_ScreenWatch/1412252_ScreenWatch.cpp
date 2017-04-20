// 1412252_ScreenWatch.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1412252_ScreenWatch.h"
#include <ctime>

#define MAX_LOADSTRING 100
#define TIMES_LENGTH	2000				//Khoảng thời gian lưu màn hình 2s
#define TIMES_REC		700					//Thời gian cho nut REC nhấp nháy 0.7s

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

int key_S, key_Ctrl, key_Shift, key_Alt;
BOOL g_ScreenWatching;                          // Trạng thái chạy lưu màn hình
BOOL g_DrawREC = true;
HWND hLabelIntro;								// Dòng thông báo giữa màn hình
WCHAR buffer[100];								
HFONT hFont;

//Các hàm cài đặt
void DoScreenWatch(HWND hWnd);									//Thực hiện tác vụ lưu màn hình
PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp);	//Tạo thông tin file Bitmap
void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi,
	HBITMAP hBMP, HDC hDC);										//Lưu file vào ổ đĩa
void errhandler(LPCTSTR sz, HWND hwnd);
void MakeBMPFileName(WCHAR bmpName[]);    //Tao tên file mới cho một file bmp
void DrawRecCircle(HDC hdc);			  //Vẽ nút REC nhấp nháy

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
    LoadStringW(hInstance, IDC_MY1412252_SCREENWATCH, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1412252_SCREENWATCH));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1412252_SCREENWATCH));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1412252_SCREENWATCH);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle,
	   WS_SYSMENU | WS_OVERLAPPED | WS_VISIBLE | WS_CLIPCHILDREN | WS_MINIMIZEBOX,
	   500, 300, 370, 100, nullptr, nullptr, hInstance, nullptr);

   SetTimer(hWnd, IDT_TIMER, TIMES_LENGTH, (TIMERPROC)NULL); //Tạo timer lưu ảnh
   SetTimer(hWnd, IDT_TIMER2, TIMES_REC, (TIMERPROC)NULL);

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
	case WM_CREATE:
		key_S = key_Ctrl = key_Shift = key_Alt = 0;
		g_ScreenWatching = FALSE;

		//Tạo thư mục chưa ảnh lưu
		CreateDirectoryW(L"ImgScr", 0);

		// Get system font
		LOGFONT lf;
		GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
		hFont = CreateFont(25, lf.lfWidth,
			lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
			lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
			lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
			lf.lfPitchAndFamily, lf.lfFaceName);

		hLabelIntro = CreateWindowEx(0, L"STATIC", L"Start watching: (Ctrl+Alt+Shift+S)",
			WS_CHILD | WS_VISIBLE | SS_LEFT, 10, 10, 300, 50, hWnd, NULL, hInst, NULL);
		SendMessage(hLabelIntro, WM_SETFONT, WPARAM(hFont), TRUE);

		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...

		if (g_ScreenWatching == TRUE)
		{
			if (g_DrawREC == TRUE)
			{
				DrawRecCircle(hdc);
			}
		}
		else
		{
			DrawRecCircle(hdc);
		}

		//DeleteObject(hPen);
		EndPaint(hWnd, &ps);
	}
	break;
    case WM_DESTROY:
		KillTimer(hWnd, IDT_TIMER);
		KillTimer(hWnd, IDT_TIMER2);
        PostQuitMessage(0);
        break;
	case WM_KEYDOWN:
		if (wParam == 0x53)
		{
			if (g_ScreenWatching == FALSE)
			{
				key_Ctrl = GetKeyState(VK_CONTROL);
				key_Shift = GetKeyState(VK_SHIFT);
				key_Alt = GetKeyState(VK_MENU);

				if ((key_Ctrl & 0x8000) && (key_Shift & 0x8000) && (key_Alt & 0x8000))
				{
					MessageBox(0, L"Watching screen", L"Screen watch", 0);
					//Thuc hien ca lenh chup man hinh o day
					g_ScreenWatching = TRUE;
					SetWindowText(hLabelIntro, L"WATCHING SCREEN    . . .  ");
					//ShowWindow(hWnd, SW_MINIMIZE);
				}
			}
			else
			{
				;//CT đang chạy
			}
			break;
		}
	case WM_TIMER:
		switch (wParam)
		{
		case IDT_TIMER:
			if (g_ScreenWatching == TRUE)
			{
				DoScreenWatch(hWnd);
			}
			break;
		case IDT_TIMER2:
			if (g_ScreenWatching == TRUE)
			{
				if (g_DrawREC == TRUE)
				{
					g_DrawREC = FALSE;
				}
				else
				{
					g_DrawREC = TRUE;
				}
				InvalidateRect(hWnd, NULL, TRUE);
				UpdateWindow(hWnd);
			}

			break;
		}
		break;
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

//Thực hiện việc lưu lại màn hình máy tính sau mỗi 2s
void DoScreenWatch(HWND hWnd)
{
	HDC ScreenDC;
	HDC memDC;
	HDC hdc;
	HBITMAP hBitmap;

	int nScrWidth;
	int nScrHeight;
	HWND hwndMainWindow;

	WCHAR fileName[100];

	ScreenDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	nScrWidth = GetDeviceCaps(ScreenDC, HORZRES);
	nScrHeight = GetDeviceCaps(ScreenDC, VERTRES);

	memDC = CreateCompatibleDC(ScreenDC);
	hBitmap = CreateCompatibleBitmap(ScreenDC, nScrWidth, nScrHeight);
	SelectObject(memDC, hBitmap);

	//Chụp ảnh màn hình lưu vào bitmap
	BitBlt(memDC, 0, 0, nScrWidth, nScrHeight, ScreenDC, 0, 0, SRCCOPY);

	////Load ảnh lên chương trình
	//hdc= GetDC(hWnd);
	//BitBlt(hdc, 0, 0, nScrWidth, nScrHeight, memDC, 0, 0, SRCCOPY);
	//UpdateWindow(hWnd);
	////MessageBox(hWnd, TEXT("File was created."), TEXT("Success"), MB_OK);

	//Xuất file

	PBITMAPINFO pbi;
	pbi = CreateBitmapInfoStruct(hWnd, hBitmap);
	MakeBMPFileName(fileName);
	CreateBMPFile(hWnd, fileName, pbi, hBitmap, memDC);

	//ReleaseDC(hWnd, hdc);
	DeleteObject((HGDIOBJ)hBitmap);
	DeleteDC(memDC);
	DeleteDC(ScreenDC);
}


//Hàm này tạo một tên file theo cấu trúng screen_yymmyy_hhmmss.bmp
void MakeBMPFileName(WCHAR bmpName[])
{
	time_t currentTime;
	struct tm localTime;

	time(&currentTime);                   // Get the current time
	localtime_s(&localTime, &currentTime);  // Convert the current time to the local time

	int Day = localTime.tm_mday;
	int Month = localTime.tm_mon + 1;
	int Year = localTime.tm_year + 1900;
	int Hour = localTime.tm_hour;
	int Min = localTime.tm_min;
	int Sec = localTime.tm_sec;

	wsprintf(bmpName, L"ImgScr\\screen_%02d%02d%02d_%02d%02d%02d.bmp", (Year % 100), Month, Day, Hour, Min, Sec);

}

//Khởi tạo thông tin file bitmap cần tạo
PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
{
	BITMAP bmp;
	PBITMAPINFO pbmi;
	WORD    cClrBits;

	// Retrieve the bitmap color format, width, and height.
	if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
		errhandler(TEXT("GetObject"), hwnd);

	// Convert the color format to a count of bits.
	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
	else if (cClrBits <= 8)
		cClrBits = 8;
	else if (cClrBits <= 16)
		cClrBits = 16;
	else if (cClrBits <= 24)
		cClrBits = 24;
	else cClrBits = 32;

	// Allocate memory for the BITMAPINFO structure. (This structure
	// contains a BITMAPINFOHEADER structure and an array of RGBQUAD
	// data structures.)

	if (cClrBits != 24)
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
			sizeof(BITMAPINFOHEADER) +
			sizeof(RGBQUAD) * (1 << cClrBits));

	// There is no RGBQUAD array for the 24-bit-per-pixel format.

	else
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
			sizeof(BITMAPINFOHEADER));

	// Initialize the fields in the BITMAPINFO structure.

	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = bmp.bmWidth;
	pbmi->bmiHeader.biHeight = bmp.bmHeight;
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
	if (cClrBits < 24)
		pbmi->bmiHeader.biClrUsed = (1 << cClrBits);

	// If the bitmap is not compressed, set the BI_RGB flag.
	pbmi->bmiHeader.biCompression = BI_RGB;

	// Compute the number of bytes in the array of color
	// indices and store the result in biSizeImage.
	// For Windows NT, the width must be DWORD aligned unless
	// the bitmap is RLE compressed. This example shows this.
	// For Windows 95/98/Me, the width must be WORD aligned unless the
	// bitmap is RLE compressed.
	pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 31) & ~31) / 8
		* pbmi->bmiHeader.biHeight;
	// Set biClrImportant to 0, indicating that all of the
	// device colors are important.
	pbmi->bmiHeader.biClrImportant = 0;
	return pbmi;
}

//Tạo file ảnh bitmap và lưu vào ổ đĩa
void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi,
	HBITMAP hBMP, HDC hDC)
{
	HANDLE hf;                 // file handle
	BITMAPFILEHEADER hdr;       // bitmap file-header
	PBITMAPINFOHEADER pbih;     // bitmap info-header
	LPBYTE lpBits;              // memory pointer
	DWORD dwTotal;              // total count of bytes
	DWORD cb;                   // incremental count of bytes
	BYTE *hp;                   // byte pointer
	DWORD dwTmp;

	pbih = (PBITMAPINFOHEADER)pbi;
	lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

	if (!lpBits)
		errhandler(TEXT("GlobalAlloc"), hwnd);

	// Retrieve the color table (RGBQUAD array) and the bits
	// (array of palette indices) from the DIB.
	if (!GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi,
		DIB_RGB_COLORS))
	{
		errhandler(TEXT("GetDIBits"), hwnd);
	}

	// Create the .BMP file.
	hf = CreateFile(pszFile,
		GENERIC_READ | GENERIC_WRITE,
		(DWORD)0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		(HANDLE)NULL);

	if (hf == INVALID_HANDLE_VALUE)
		errhandler(TEXT("CreateFile"), hwnd);
	hdr.bfType = 0x4d42;        // 0x42 = "B" 0x4d = "M"
								// Compute the size of the entire file.
	hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD) + pbih->biSizeImage);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;

	// Compute the offset to the array of color indices.
	hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +
		pbih->biSize + pbih->biClrUsed
		* sizeof(RGBQUAD);

	// Copy the BITMAPFILEHEADER into the .BMP file.
	if (!WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER), (LPDWORD)&dwTmp, NULL))
	{
		errhandler(TEXT("WriteFile"), hwnd);
	}

	// Copy the BITMAPINFOHEADER and RGBQUAD array into the file.
	if (!WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER)
		+ pbih->biClrUsed * sizeof(RGBQUAD),
		(LPDWORD)&dwTmp, (NULL)))
		errhandler(TEXT("WriteFile"), hwnd);

	// Copy the array of color indices into the .BMP file.
	dwTotal = cb = pbih->biSizeImage;
	hp = lpBits;
	if (!WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL))
		errhandler(TEXT("WriteFile"), hwnd);

	// Close the .BMP file.
	if (!CloseHandle(hf))
		errhandler(TEXT("CloseHandle"), hwnd);

	// Free memory.
	GlobalFree((HGLOBAL)lpBits);
}
void errhandler(LPCTSTR sz, HWND hwnd)
{
	MessageBox(hwnd, sz, L"Error", MB_ICONERROR);
	PostQuitMessage(0);
}

//Tạo nút REC màu đỏ nhấp nháy
void DrawRecCircle(HDC hdc)
{
	HRGN hRegion = CreateEllipticRgn(310, 5, 340, 35);
	HBRUSH hbrush = CreateSolidBrush(RGB(255, 0, 0));
	FillRgn(hdc, hRegion, hbrush);
}
