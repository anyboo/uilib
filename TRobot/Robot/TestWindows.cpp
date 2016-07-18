#include "TestWindows.h"
#include <Poco/SingletonHolder.h>


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

TestWindows::TestWindows()
{
	m_hInst = GetModuleHandle(NULL);
}


TestWindows::~TestWindows()
{
}

TestWindows& TestWindows::getInstance()
{
	static Poco::SingletonHolder<TestWindows> shTestWindows;
	return *shTestWindows.get();
}


void TestWindows::Init()
{
	MyRegisterClass();
	InitInstance();
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
	m_hWnd = CreateWindow(TEXT("Test"), TEXT("Test"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, m_hInst, NULL);

	if (!m_hWnd)
	{
		return FALSE;
	}

	/*ShowWindow(m_hWnd, SW_SHOW);*/

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