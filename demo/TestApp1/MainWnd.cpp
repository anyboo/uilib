#include "StdAfx.h"
#include "SettingWnd.h"
#include "AboutWnd.h"
#include "MainWnd.h"

CMainWnd::CMainWnd()
{
	Page1.SetPaintMagager(&m_PaintManager);
	Page2.SetPaintMagager(&m_PaintManager);

	AddVirtualWnd(_T("page1"), &Page1);
	AddVirtualWnd(_T("page2"), &Page2);
}

CMainWnd::~CMainWnd()
{
	RemoveVirtualWnd(_T("page1"));
	RemoveVirtualWnd(_T("page2"));
}

DUI_BEGIN_MESSAGE_MAP(CMainWnd, WindowImplBase)
	DUI_ON_CLICK_CTRNAME(CTR_ABOUT, OnAbout)
	DUI_ON_CLICK_CTRNAME(CTR_SETTING, OnSettingDlg)
	DUI_ON_CLICK_CTRNAME(CTR_MIN, OnMin)
	DUI_ON_CLICK_CTRNAME(CTR_CLOSE, OnClose)
DUI_END_MESSAGE_MAP()

LPCTSTR CMainWnd::GetWindowClassName() const
{
	return _T("MainWnd");
}

CDuiString CMainWnd::GetSkinFolder()
{
	return _T("skin\\");
}

CDuiString CMainWnd::GetSkinFile()
{
	return _T("record.xml");
}

void CMainWnd::OnAbout(TNotifyUI& msg)
{
	CAboutWnd Dlg;

	Dlg.Create(GetHWND(), NULL, UI_WNDSTYLE_DIALOG, 0L, 0, 0, 500, 400);
	Dlg.CenterWindow();
	Dlg.ShowModal();
}

void CMainWnd::OnSettingDlg(TNotifyUI& msg)
{
	CSettingWnd SettingDlg;

	SettingDlg.Create(GetHWND(), NULL, UI_WNDSTYLE_DIALOG, 0L, 0, 0, 500, 400);
	SettingDlg.CenterWindow();
	SettingDlg.ShowModal();
}

void CMainWnd::OnMin(TNotifyUI& msg)
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainWnd::OnClose(TNotifyUI& msg)
{
	::PostQuitMessage(0L);
	//Close();
}

void CMainWnd::Notify(TNotifyUI& msg)
{
	WindowImplBase::NotifyPump(msg);
}

#include "Setting.h"
void CMainWnd::InitWindow()
{
	std::wstring current;
	CSetting::Inst().GetEncode(current);

	CPaintManagerUI& pm = m_PaintManager;
	CButtonUI* control = dynamic_cast<CButtonUI*>(pm.FindControl(_T("btencode")));
	if (!control) return;
	control->SetHotBkColor(0);

	/*
	control->SetAttribute(L"current", current.c_str());
	CDuiString imagePath = control->GetAttribute(current.c_str());
	control->SetNormalImage(imagePath);
	*/

}

void CMainWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	//delete this;
}

LRESULT CMainWnd::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_ESCAPE)
		return true;
	return WindowImplBase::ResponseDefaultKeyEvent(wParam);
}