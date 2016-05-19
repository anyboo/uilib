// App.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "resource.h"
#include "MainWnd.h"
#include "SettingWnd.h"

void CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime)
{
	SendMessage(hWnd, WM_TIMER, NULL, NULL);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));

	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;

	CMainWnd* pFrame = new CMainWnd();
	if (pFrame == NULL) return 0;
	pFrame->Create(NULL, NULL, UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES);
	pFrame->SetIcon(IDI_ICON1);
	pFrame->CenterWindow();
	pFrame->ShowWindow(true);

	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	return 0;
}
