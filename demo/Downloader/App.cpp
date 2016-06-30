// App.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "MainWnd.h"
#include "resource.h"


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("..//x86"));

	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;

	CMainWnd* pFrame = new CMainWnd();
	pFrame->Create(NULL, NULL, UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES);
	pFrame->SetIcon(IDI_ICON1);
	pFrame->CenterWindow();
	pFrame->ShowWindow(true);

	LPARAM lParam;
	lParam = ABS_AUTOHIDE | ABS_ALWAYSONTOP;
	HWND task = FindWindow(_T("Shell_TrayWnd"), NULL);
	::ShowWindow(task, SW_HIDE);

	APPBARDATA apBar;
	memset(&apBar, 0, sizeof(apBar));
	apBar.cbSize = sizeof(apBar);
	apBar.hWnd = task;
	if (apBar.hWnd != NULL)
	{
		apBar.lParam = lParam;
		SHAppBarMessage(ABM_SETSTATE, &apBar);
	}
	
	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	return 0;
}


