// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include <ctime>

#ifdef LIBRARY_EXPORTS
#    define LIBRARY_API __declspec(dllexport)
#else
#    define LIBRARY_API __declspec(dllimport)
#endif

#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)


HWND hWnd;
HHOOK hMyHook;
HINSTANCE hInstance;

clock_t g_Time_hook ;
clock_t g_Time_nexthook;
#define TIME_ALLOW_HOOK 100      //Khoảng thời gian tối thiểu để thực hiện hook lần kế tiếp



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	hInstance = hModule;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

LRESULT CALLBACK KeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);
 //INT InitKeyBoardHook(HWND);
 //INT UninstallKeyBoardHook();

 EXTERN_DLL_EXPORT INT InitKeyBoardHook(HWND hWnd_main)
{
	 g_Time_hook = clock();
	
	hWnd = hWnd_main;
	hMyHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyBoardProc, hInstance, 0);
	if (hMyHook != NULL)
	{	
		
		return 1;
	}	
	return 0;
}

EXTERN_DLL_EXPORT INT UninstallKeyBoardHook()
{
	if (hMyHook != NULL)
	{
		UnhookWindowsHookEx(hMyHook);
		return 1;
	}
	return 0;
}

LRESULT CALLBACK KeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	WCHAR buffer[256];
	if (nCode == HC_ACTION) 
	{		
		
		if( WM_KEYUP == wParam )
		{			
			int LeftWin = GetKeyState(VK_LWIN);
			int RightWin = GetKeyState(VK_RWIN);
			int CtrtKey = GetKeyState(VK_CONTROL);
			
			PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;	
			
			g_Time_nexthook = clock(); 
			if ((p->vkCode == VK_SPACE) && (CtrtKey & 0x8000 ) )
			{		
				if (IsWindowVisible(hWnd))
				{					
					ShowWindow(hWnd, SW_HIDE);					
				}
				else
				{
					ShowWindow(hWnd, SW_RESTORE);
				}
			}
		}
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}
