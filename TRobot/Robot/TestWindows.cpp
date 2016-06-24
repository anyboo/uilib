#include "stdafx.h"
#include "TestWindows.h"


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HWND g_hWnd = NULL;

TestWindows::TestWindows()
{
	m_hInst = GetModuleHandle(NULL);
}


TestWindows::~TestWindows()
{
}

int TestWindows::Init()
{
	this->MyRegisterClass();
	this->InitInstance();
// 	{
// 		return 0;
// 	}

// 	MSG msg;
// 
// 	while (GetMessage(&msg, NULL, 0, 0))
// 	{
// 		TranslateMessage(&msg);
// 		DispatchMessage(&msg);
// 		
// 	}
	return 0;
}

ATOM TestWindows::MyRegisterClass()
{
	WNDCLASS ws;

	ws.cbClsExtra = 0;
	ws.cbWndExtra = 0;
	ws.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	ws.hCursor = LoadCursor(m_hInst, IDC_ARROW);
	ws.hIcon = LoadIcon(m_hInst, IDI_APPLICATION);
	ws.hInstance = m_hInst;
	ws.lpfnWndProc = WndProc;
	ws.lpszClassName = TEXT("Test");
	ws.lpszMenuName = NULL;
	ws.style = CS_HREDRAW | CS_VREDRAW;

	return RegisterClass(&ws);
}

BOOL TestWindows::InitInstance()
{
	g_hWnd = CreateWindow(TEXT("Test"), TEXT("Test"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, m_hInst, NULL);

	if (!g_hWnd)
	{
		return FALSE;
	}

	ShowWindow(g_hWnd, SW_SHOW);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{


	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);;
}