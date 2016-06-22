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
	this->MyRegisterClass(NULL);
	if (!this->InitInstance())
	{
		return 0;
	}

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		
	}

	return msg.wParam;
}

ATOM TestWindows::MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASS ws;

	ws.cbClsExtra = 0;
	ws.cbWndExtra = 0;
	ws.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	ws.hCursor = NULL;
	ws.hIcon = NULL;
	ws.hInstance = m_hInst;
	ws.lpfnWndProc = WndProc;
	ws.lpszClassName = TEXT("Test");
	ws.lpszMenuName = TEXT("Test");
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

	ShowWindow(g_hWnd, 0);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择: 
		switch (wmId)
		{
		case WM_CLOSE:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}