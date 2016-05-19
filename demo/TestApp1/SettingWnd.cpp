#include "stdafx.h"
#include "SettingWnd.h"
#include "resource.h"

CSettingWnd::CSettingWnd()
{
	RegisterWindowClass();
}

CSettingWnd::~CSettingWnd()
{
}

void CSettingWnd::InitWindow()
{

}

void CSettingWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CSettingWnd::Notify(TNotifyUI& msg)
{
	WindowImplBase::Notify(msg);
}


DUI_BEGIN_MESSAGE_MAP(CSettingWnd, WindowImplBase)
	/*DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK, OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED, OnSelectChanged)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_ITEMCLICK, OnItemClick)*/
DUI_END_MESSAGE_MAP()

LPCTSTR CSettingWnd::GetWindowClassName() const
{
	return _T("SettingWnd");
}

CDuiString CSettingWnd::GetSkinFolder()
{
	return _T("skin\\");
}

CDuiString CSettingWnd::GetSkinFile()
{
	return _T("SettingDlg.xml");
}