#include "stdafx.h"
#include "Setting.h"
#include "CaptureWnd.h"


CCaptureWnd::CCaptureWnd()
:config("config.json")
{
	config.load();
}

CCaptureWnd::~CCaptureWnd()
{
	config.save();
}

DUI_BEGIN_MESSAGE_MAP(CCaptureWnd, WindowImplBase)
//DUI_ON_MSGTYPE()
DUI_END_MESSAGE_MAP()

CDuiString CCaptureWnd::GetSkinFolder()
{
	return _T("SettingWnd");
}

CDuiString CCaptureWnd::GetSkinFile()
{
	return _T("Capture.xml");
}

LPCTSTR CCaptureWnd::GetWindowClassName(void) const
{
	return _T("CaptureWnd");
}

void CCaptureWnd::InitWindow()
{
	CPaintManagerUI& pm = m_PaintManager;
	Image = dynamic_cast<CControlUI*>(pm.FindControl(_T("desktopimage")));
	Mask = dynamic_cast<CControlUI*>(pm.FindControl(_T("desktopmask")));
	MaskBorder = dynamic_cast<CControlUI*>(pm.FindControl(_T("desktopmaskborder")));
	CanvasContainer = dynamic_cast<CContainerUI*>(pm.FindControl(_T("desktopcanvascontainer")));

	assert(Image || Mask || MaskBorder || CanvasContainer);

	::GetWindowRect(::GetDesktopWindow(), &Window);

	POINT p = { Window.left, Window.top };
	size_t width = Window.right - Window.left;
	size_t height = Window.bottom - Window.top;
	
	::SetWindowPos(GetHWND(), NULL, p.x, p.y, width, height, SWP_NOZORDER);

	/*
	***missing CreateDesktopBitmap CreateDesktopMaskBitmap***

	HBITMAP hDesktopBmp = CreateDesktopBitmap(GetHWND());
	pm.AddImage(_T("desktopimage"), hDesktopBmp,width, height, false);
	HBITMAP hDesktopBmpMask = CreateDesktopMaskBitmap(GetHWND());
	pm.AddImage(_T("desktopimagemask"), hDesktopBmpMask, width, height, true);
	*/

	Image->SetBkImage(_T("desktopimage"));
	Mask->SetBkImage(_T("desktopimagemask"));
}

void CCaptureWnd::Notify(TNotifyUI& msg)
{
	WindowImplBase::Notify(msg);
}

LRESULT CCaptureWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_LBUTTONDOWN: 
		//OnLBUTTONDOWN();	
		break;
	case WM_LBUTTONUP:
		//OnLBUTTONUP();
		break;
	case WM_LBUTTONDBLCLK:
		//OnLBUTTONDBLCLK();
		break;
	case WM_RBUTTONUP:
		//OnRBUTTONUP();
		break;
	case WM_MOUSEMOVE:
		//OnMOUSEMOVE();
		break;
	default:
		break;
	}

	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}