#include "stdafx.h"
#include "AboutWnd.h"
#include "resource.h"

CAboutWnd::CAboutWnd()
{
	RegisterWindowClass();
}

CAboutWnd::~CAboutWnd()
{
}

void CAboutWnd::InitWindow()
{

}

void CAboutWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CAboutWnd::Notify(TNotifyUI& msg)
{
	WindowImplBase::Notify(msg);
}


DUI_BEGIN_MESSAGE_MAP(CAboutWnd, WindowImplBase)
	/*DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK, OnItemClick)*/
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