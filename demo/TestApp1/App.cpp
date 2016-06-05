// App.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "MainWnd.h"
#include "resource.h"
#include <Poco/NamedMutex.h>

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
	Poco::NamedMutex unique("SRTool");
	if (!unique.tryLock())
		return 0;

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));

	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;

	CMainWnd Frame;
	Frame.Create(NULL, NULL, UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES);
	Frame.SetIcon(IDI_ICON1);
	Frame.CenterWindow();
	Frame.ShowWindow(true);
	
	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	return 0;
}
