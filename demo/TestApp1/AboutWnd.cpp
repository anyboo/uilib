#include "stdafx.h"
#include "AboutWnd.h"

CAboutWnd::CAboutWnd()
{
}

CAboutWnd::~CAboutWnd()
{
}

void CAboutWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	//delete this;
}

void CAboutWnd::Notify(TNotifyUI& msg)
{
	WindowImplBase::Notify(msg);
}


DUI_BEGIN_MESSAGE_MAP(CAboutWnd, WindowImplBase)
DUI_END_MESSAGE_MAP()

LPCTSTR CAboutWnd::GetWindowClassName() const
{
	return _T("AboutWnd");
}

CDuiString CAboutWnd::GetSkinFolder()
{
	return _T("skin\\");
}

CDuiString CAboutWnd::GetSkinFile()
{
	return _T("AboutDlg.xml");
}