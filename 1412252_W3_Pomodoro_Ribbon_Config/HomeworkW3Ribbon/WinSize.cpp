#include "stdafx.h"
#include "WinSize.h"


CWinSize::CWinSize()
{
	WCHAR buffer[BUFFERSIZE];
	WCHAR curPath[BUFFERSIZE];	

	// Tạo đường dẫn tuyệt đối tới file config
	GetCurrentDirectory(BUFFERSIZE, curPath);
	wsprintf(configPath, L"%s\\config.ini", curPath);

	//Đọc thông tin từ file config
	GetPrivateProfileString(L"winsize", L"posX", L"0", buffer, BUFFERSIZE, configPath);
	pX = _wtoi(buffer);
	GetPrivateProfileString(L"winsize", L"posY", L"0", buffer, BUFFERSIZE, configPath);
	pY = _wtoi(buffer);
	GetPrivateProfileString(L"winsize", L"width", L"300", buffer, BUFFERSIZE, configPath);
	Width = _wtoi(buffer);
	GetPrivateProfileString(L"winsize", L"height", L"300", buffer, BUFFERSIZE, configPath);
	Height = _wtoi(buffer);		
	
}


CWinSize::~CWinSize()
{
}

//Get the screen coordinates information from Config file
bool CWinSize::LoadWinSizeformConfig(HWND hWnd)
{
	
	return SetWindowPos(hWnd, 0, pX, pY, Width, Height, NULL);
}

//Get coordinates when there is a resize or move
bool CWinSize::PostWinSizeChange(HWND hWnd)
{
	LPRECT WinCurPostion = new RECT; //Storage postion of window at the moment
	long left, right, top, bottom;

	GetWindowRect(hWnd, WinCurPostion);
	left = WinCurPostion->left;
	right = WinCurPostion->right;
	top = WinCurPostion->top;
	bottom = WinCurPostion->bottom;

	pX = left;
	pY = top;
	Width = right - left;
	Height = bottom - top;	
	
	
	/*WCHAR buffer[256];
	wsprintf(buffer, L"x=%d y=%d width=%d height=%d", pX, pY, Width, Height);
	MessageBox(0, buffer, 0, 0);*/

	this->SavetoConfig();
	delete WinCurPostion;
	return true;
}

//Save coordinates to Config file
bool CWinSize::SavetoConfig()
{
	WCHAR buffer[BUFFERSIZE];

	wsprintf(buffer, L"%d", pX);
	WritePrivateProfileString(L"winsize", L"posX", buffer, configPath);
	wsprintf(buffer, L"%d", pY);
	WritePrivateProfileString(L"winsize", L"posY", buffer, configPath);
	wsprintf(buffer, L"%d", Width);
	WritePrivateProfileString(L"winsize", L"width", buffer, configPath);
	wsprintf(buffer, L"%d", Height);
	WritePrivateProfileString(L"winsize", L"height", buffer, configPath);
	return true;
}

int CWinSize::getpX()
{
	return pX;
}
int CWinSize::getpY()
{
	return pY;
}
int CWinSize::getWidth()
{
	return Width;
}
int CWinSize::getHeight()
{
	return Height;
}