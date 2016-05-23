#include "stdafx.h"
#include "CaptureWnd.h"


CCaptureWnd::CCaptureWnd()
{
}


CCaptureWnd::~CCaptureWnd()
{
}


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