// App.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Launcher.h"

static void LoadConfiguration();
static void SaveConfiguration();

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{
    CPaintManagerUI::SetInstance(hInstance);
    CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin") );

	LoadConfiguration();
    HRESULT Hr = ::CoInitialize(NULL);
    if( FAILED(Hr) ) return 0;

	Launcher* pFrame = new Launcher();
    if( pFrame == NULL ) return 0;
	pFrame->Create(NULL, NULL, UI_WNDSTYLE_DIALOG , WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES | WS_EX_STATICEDGE | WS_EX_APPWINDOW);
	pFrame->SetIcon(IDI_ICON);
    pFrame->CenterWindow();
    pFrame->ShowWindow(true);
    CPaintManagerUI::MessageLoop();
	SaveConfiguration();
    ::CoUninitialize();
    return 0;
}

void LoadConfiguration()
{
	//Load configuration from json file
	//configuration is path of shortcut
}

void SaveConfiguration()
{

}