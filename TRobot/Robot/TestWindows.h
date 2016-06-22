#pragma once

#include <Windows.h>

extern HWND g_hWnd;

class TestWindows
{
public:
	TestWindows();
	~TestWindows();

	int Init();

private:
	ATOM MyRegisterClass(HINSTANCE hInstance);
	BOOL InitInstance();

private:
	HINSTANCE m_hInst;
};

