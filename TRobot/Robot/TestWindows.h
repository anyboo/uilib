#pragma once

#include <Windows.h>


class TestWindows
{
public:
	TestWindows();
	~TestWindows();

	static TestWindows& getInstance();
	void Init();

	ATOM MyRegisterClass();
	BOOL InitInstance();

	HWND GetHWnd(){ return m_hWnd; }
	void HideWindow(){ ShowWindow(m_hWnd, SW_HIDE); }
	void DisplayWindow(){ ShowWindow(m_hWnd, SW_SHOW); }

private:
	HWND m_hWnd;
	HINSTANCE m_hInst;
};

