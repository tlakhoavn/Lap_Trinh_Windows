#pragma once


#define BUFFERSIZE 256
class CWinSize
{
private:
	int pX;
	int pY;
	int Height;
	int Width;
	WCHAR configPath[BUFFERSIZE];
public:
	bool LoadWinSizeformConfig(HWND hWnd);          
	bool PostWinSizeChange(HWND hWnd);
	bool SavetoConfig();
	CWinSize();
	~CWinSize();

	int getpX();
	int getpY();
	int getWidth();
	int getHeight();
};

