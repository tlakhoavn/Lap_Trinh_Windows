//Implement the main functions of the Pomodoro program
#pragma once
#include "Resource.h"

#define MIN_WIN_WIDTH_SIZE 490
#define MIN_WIN_HEIGHT_SIZE 280

#define short_breaktime_item1 3
#define short_breaktime_item2 4
#define short_breaktime_item3 5

#define long_breaktime_item1  10
#define long_breaktime_item2  15
#define long_breaktime_item3  20
#define long_breaktime_item4  25

#define worktime_item1        25
static int STime_ = 0;
static bool ON_COUNT = false;

class CPomodoro
{
private:
	HWND	hwnd_edt_Work,
		hWnd_stt_task,
		hWnd_Stext_Clock,
		hWnd_Duration,
		h_Thread_ShowClock;
		/*hwnd_btn_Start, hwnd_btn_Abandon, hWnd_Stext_SBr,
		hwnd_cb_ShortBreak, hWnd_Stext_LBr, hwnd_cb_LongBreak;*/

	bool OnDuration;                    //Flag to indicate the clock running, default when create is FALSE
	int CountofWork;                    //Flag to indicate the time when work | OFF = 0, ON = 1,2,3,4 -> after four time set a long break
	bool OnBreakTime;					//Flag to indicate the time when rest

	int St_BreakType;		//3 types			//Type to choice time of break duration 
	int Lg_BreakType;       //4 types
public:
	CPomodoro();
	~CPomodoro();
	bool Create(HWND hWnd, HINSTANCE hInstance);			//Create item on main windown
	bool RePaintWindow(HWND hWnd, HINSTANCE hInstance);		//Redraw the screen when there is a change in size, window
	bool ShowTimeCountDown(int Second_);                    //Show countdown time clock
	void SetShortTimeBreak(int Type);
	void SetLongTimeBreak(int Type);

	bool StartPomodoro();
	bool AbandonPomodoro();
	bool BreakTimePomodoro();
	void Reset();               //Reset setting for a new Pomodoro
	void KillTimeDuration();    //Kill pomodoro timer when  time up or Abandon event
	void SetTimeHandle(HWND hWnd);
	void StartThreadShowTime(HWND hWnd, int second_);
	
};

