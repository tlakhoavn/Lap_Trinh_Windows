#include "stdafx.h"
#include "Pomodoro.h"


CPomodoro::CPomodoro()
{
	OnDuration = TRUE;
	CountofWork = 0;
	OnBreakTime = FALSE;
	St_BreakType = 2;
	Lg_BreakType = 1;
}
CPomodoro::~CPomodoro()
{
}

bool CPomodoro::Create(HWND hWnd, HINSTANCE hInst)
{

	
	// Lấy font hệ thống
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);
	

	hWnd_stt_task = CreateWindowEx(0, L"STATIC", L"Add your task here and click START", WS_CHILD | WS_VISIBLE | SS_LEFT ,
		0, 0, 0, 0, hWnd, NULL, hInst, NULL);
	SendMessage(hWnd_stt_task, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd_edt_Work = CreateWindowEx(0, L"EDIT", L"",
		WS_CHILD | WS_VISIBLE | WS_BORDER ,
		0, 0, 0, 0, hWnd, (HMENU)IDC_EDT_ADDTASK, hInst, NULL);
	SendMessage(hwnd_edt_Work, WM_SETFONT, WPARAM(hFont), TRUE);

	hWnd_Stext_Clock = CreateWindowEx(0, L"STATIC", L"", WS_CHILD | WS_VISIBLE | SS_CENTER,
		0, 0, 0, 0, hWnd, NULL, hInst, NULL);
	SendMessage(hWnd_Stext_Clock, WM_SETFONT, WPARAM(hFont), TRUE);

	/*hWnd_Stext_Status = CreateWindowEx(0, L"STATIC", L"Welcome to Pomodoro.", WS_CHILD | WS_VISIBLE | SS_CENTER,
	100, 270, 250, 20, hWnd, NULL, hInst, NULL);*/
	//SendMessage(hWnd_Stext_Status, WM_SETFONT, WPARAM(hFont), TRUE);

	/*hwnd_btn_Start = CreateWindowEx(0, L"BUTTON", L"START", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 250, 100, 40, hWnd, (HMENU)IDC_BTN_START, hInst, NULL);
	SendMessage(hwnd_btn_Start, WM_SETFONT, WPARAM(hFont), TRUE);

	hwnd_btn_Abandon = CreateWindowEx(0, L"BUTTON", L"ABANDON", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		200, 250, 100, 40, hWnd, (HMENU)IDC_BTN_ABANDON, hInst, NULL);
	SendMessage(hwnd_btn_Abandon, WM_SETFONT, WPARAM(hFont), TRUE);*/

	//hWnd_Stext_SBr = CreateWindowEx(0, L"STATIC", L"Short break duration:", WS_CHILD | WS_VISIBLE | SS_LEFT,
	//	350, 154, 200, 20, hWnd, NULL, hInst, NULL);
	//SendMessage(hWnd_Stext_SBr, WM_SETFONT, WPARAM(hFont), TRUE);

	//hwnd_cb_ShortBreak = CreateWindowEx(0,L"COMBOBOX", TEXT("ShortBreak"),
	//	CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
	//	350, 175, 200, 20, hWnd, NULL, hInst, NULL);
	//SendMessage(hwnd_cb_ShortBreak, WM_SETFONT, WPARAM(hFont), TRUE);


	//hWnd_Stext_LBr = CreateWindowEx(0, L"STATIC", L"Long break duration:", WS_CHILD | WS_VISIBLE | SS_LEFT,
	//	350, 204, 200, 20, hWnd, NULL, hInst, NULL);
	//SendMessage(hWnd_Stext_LBr, WM_SETFONT, WPARAM(hFont), TRUE);

	//hwnd_cb_LongBreak = CreateWindowEx(0,L"COMBOBOX", TEXT("LongBreak"),
	//	CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
	//	350, 225, 200, 20, hWnd, NULL, hInst, NULL); //125
	//SendMessage(hwnd_cb_LongBreak, WM_SETFONT, WPARAM(hFont), TRUE);

	this->RePaintWindow(hWnd, hInst);	
	return true;
}

//Redraw the screen when there is a change in size, window
bool CPomodoro::RePaintWindow(HWND hWnd, HINSTANCE hInstance)
{
	//return FALSE;
	int const ribbonHeight = 150;         //Độ cao khung ribbon
	float const RatioWidth = 0.6;
	float const RatioHeight = 0.6;
	int const stt_task_Height = 20;
	int const edt_Stext_Height = 20;

	int edt_Work_X, edt_Work_Y, edt_Work_Heigth, edt_Work_Width = 0;
	int WindowWidth, WindowHeight;

	LPRECT WinCurPostion = new RECT; //Storage postion of window at the moment

									 //Lấy vị trí hiện tại của window
	GetWindowRect(hWnd, WinCurPostion);
	WindowWidth = WinCurPostion->right - WinCurPostion->left;
	WindowHeight = WinCurPostion->bottom - WinCurPostion->top;
	

	//Set vị trí tự động cho các item
	edt_Work_X = WindowWidth*(1 - RatioWidth) / 2;
	edt_Work_Y = (WindowHeight - ribbonHeight)*(1 - RatioHeight) / 2 + ribbonHeight;
	edt_Work_Width = WindowWidth*RatioWidth;
	edt_Work_Heigth = (WindowHeight - ribbonHeight)*RatioHeight;

	//DestroyWindow(hWnd_stt_task);
	//DestroyWindow(hwnd_edt_Work);
	//DestroyWindow(hWnd_Stext_Clock);

	SetWindowPos(hWnd_stt_task, NULL, edt_Work_X, edt_Work_Y - stt_task_Height, edt_Work_Width, edt_Stext_Height,NULL);
	SetWindowPos(hwnd_edt_Work, NULL, edt_Work_X, edt_Work_Y, edt_Work_Width, edt_Work_Heigth, NULL);
	SetWindowPos(hWnd_Stext_Clock, NULL, edt_Work_X, edt_Work_Y + edt_Work_Heigth + edt_Stext_Height, edt_Work_Width, edt_Stext_Height, NULL);
	
	delete WinCurPostion;
	return true;
}

//Show countdown time clock
bool CPomodoro::ShowTimeCountDown(int Second_)
{	
	/*WCHAR buffer[30];
	
	for (int i = Second_; i >=0; i--)
	{
		if (OnDuration == TRUE)
		{
			wsprintfW(buffer, L"%d minutes %d second", i/60, i%60);
			if (SetWindowTextW(this->hWnd_Stext_Clock, buffer) == true)
			{
				MessageBox(0, L"true", 0, 0);
			}
			else MessageBox(0, L"false", 0, 0);
		}
		else
		{
			return false;
		}
		Sleep(1000);
	}*/


	return true;
}

void CPomodoro::SetShortTimeBreak(int Type)
{
	int TypeBreak = 1;	switch (Type)
	{
	case 1:
		if (1 == this->St_BreakType)
		{
			MessageBox(0, L"This object has been previously selected", L"NOTIFICATION", MB_OK);
			return;
		}			
		break;
	case 2:
		if (2 == this->St_BreakType)
		{
			MessageBox(0, L"This object has been previously selected", L"NOTIFICATION", MB_OK);
			return;
		}			
		break;	
	case 3: 
		if (3 == this->St_BreakType)
		{
			MessageBox(0, L"This object has been previously selected", L"NOTIFICATION", MB_OK);
			return;
		}		
		break;
	default: return;
	}

	if (true == MessageBox(0, L"Do you want to change?", L"Question", MB_OKCANCEL))
	{
		St_BreakType = Type;
	}
}

void CPomodoro::SetLongTimeBreak(int Type)
{
	int TypeBreak = 1;
	switch (Type)
	{
	case 1:
		if (1 == this->Lg_BreakType)
		{
			MessageBox(0, L"This object has been previously selected", L"NOTIFICATION", MB_OK);
			return;
		}
		else
		{
			TypeBreak = 1;
		}
		break;
	case 2:
		if (2 == this->Lg_BreakType)
		{
			MessageBox(0, L"This object has been previously selected", L"NOTIFICATION", MB_OK);
			return;
		}
		else
		{
			TypeBreak = 2;
		}
		break;
	case 3:
		if (3 == this->Lg_BreakType)
		{
			MessageBox(0, L"This object has been previously selected", L"NOTIFICATION", MB_OK);
			return;
		}
		else
		{
			TypeBreak = 3;
		}
		break;
	case 4:
		if (4 == this->Lg_BreakType)
		{
			MessageBox(0, L"This object has been previously selected", L"NOTIFICATION", MB_OK);
			return;
		}
		else
		{
			TypeBreak = 4;
		}
		break;
	}

	if (true == MessageBox(0, L"Do you want to change?", L"Question", MB_OKCANCEL))
	{
		Lg_BreakType = TypeBreak;
	}
}

bool CPomodoro::StartPomodoro()
{
	if ( this->CountofWork >0)
	{
		MessageBox(0, L"Your pomodoro is running. Don't start again. Click Abandon if you want to Stop.",L"POMODOR START",0);
		return false;
	}
	else
	{
		if (true == MessageBox(0, L"Do you want to Start your Pomodoro?", L"POMODORO START", MB_OKCANCEL))
		{
			this->OnBreakTime = true;
			//Do Start here
			this->BreakTimePomodoro();
		}
		else
		{
			return false;
		}				
	}
	return true;
}

bool CPomodoro::AbandonPomodoro()
{
	if ((0 == CountofWork) && (false == OnBreakTime))
	{
		MessageBox(0, L"No Pomodoro is running at the moment. Click Start to begin a Pomodoro.", L"Notification", MB_OK);
		return false;
	}
	else
	{
		if (true == MessageBox(0, L"Do you want to Stop your Pomodoro? Your Pomodoro will be refreshed.", L"POMODORO ABANDON", MB_OKCANCEL))
		{
			this->CountofWork = 0;
			this->OnBreakTime = false;
			//Do Stop here
			this->Reset();
			this->KillTimeDuration();
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool CPomodoro::BreakTimePomodoro()
{
	ON_COUNT = true;
	HWND hWnd = GetForegroundWindow();
	int Time_ = 5000;         //time in milion seconds
	WCHAR buffer[256];

	if (false == this->OnBreakTime)
	{			
		this->OnBreakTime = true;

		if (3 == (this->CountofWork %4)) //After four work -> 1 long break
		{			
			//Do long break;
			switch (Lg_BreakType)
			{
			case 1:
				Time_ = long_breaktime_item1;
				break;
			case 2:
				Time_ = long_breaktime_item2;
				break;
			case 3:
				Time_ = long_breaktime_item3;
				break;
			case 4:
				Time_ = long_breaktime_item4;
				break;
			default:
				Time_ = long_breaktime_item1;
			}
			wsprintf(buffer, L"Your pomodoro is over. It's time to take a long break in %d minutes", Time_);
			MessageBox(0, buffer, L"Pomorodo", 0);
			
		}
		else
		{			
			//Do short break;
			switch (St_BreakType)
			{
			case 1:
				Time_ = short_breaktime_item1;
				break;
			case 2:
				Time_ = short_breaktime_item2;
				break;
			case 3:
				Time_ = short_breaktime_item3;
				break;
			default:
				Time_ = short_breaktime_item2;		
			}
			wsprintf(buffer, L"Your pomodoro is over. It's time to take a short break in %d minutes", Time_);
			MessageBox(0, buffer, L"Pomorodo", 0);
		}
	}
	else
	{
		this->CountofWork++;
		this->OnBreakTime = false;

		if (this->CountofWork >1)
		{
			MessageBox(0, L"Countinue your job with 25 Minutes Pomodoro", L"Pomodoro", 0);
		}
		else
		{
			MessageBox(0, L"Enjoy your job with 25 Minutes Pomodoro", L"Pomodoro", 0);
		}
		Time_ = worktime_item1;

		/*wsprintf(buffer, L"WORK-Onworktime=%d", CountofWork);
		MessageBox(0, buffer, 0, 0);*/
		
	}	
	
	this->StartThreadShowTime(hWnd, Time_);	
	return true;
}

void CPomodoro::Reset()
{
	this->OnBreakTime = 0;
	this->CountofWork = 0;
	this->OnDuration = 0;
	ON_COUNT = false;
}
//Kill pomodoro timer when  time up or Abandon event
void CPomodoro::KillTimeDuration()
{
	HWND hWnd = GetForegroundWindow();
	KillTimer(hWnd_Duration, IDT_TIMER_DURATION);
}
void CPomodoro::SetTimeHandle(HWND hWnd)
{
	hWnd_Duration = hWnd;
}

void ShowTimer()
{
	for (int i = 0; i <= STime_; i++)
	{
		if (ON_COUNT == false)return;
		Sleep(1000);
	}
	SendMessage(GetForegroundWindow(), WM_TIMER, IDT_TIMER_DURATION, 0);
	
}
void CPomodoro::StartThreadShowTime(HWND hWnd, int second_)
{	
	void(*foo)() = &ShowTimer;
	STime_ = second_;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)foo, NULL, 0, NULL);
}