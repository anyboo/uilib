#include "stdafx.h"
#include "MainWnd.h"
#include "DownLoadWnd.h"
#include "LogUI.h"
#include "VideoLoginUI.h"
#include "OtherTools.h"

CMainWnd::CMainWnd()
:m_IsMinWnd(FALSE)
{
	
}


CMainWnd::~CMainWnd()
{
	
}


DUI_BEGIN_MESSAGE_MAP(CMainWnd, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CLOSE, OnClose)
DUI_ON_CLICK_CTRNAME(BT_MINWIND, OnMin)
DUI_ON_CLICK_CTRNAME(BT_DOWNLOAD, OnDownLoadWnd)
DUI_ON_CLICK_CTRNAME(BT_LogWnd, OnLogWnd)
DUI_ON_CLICK_CTRNAME(BT_OtherTools, OnOtherToolsWnd)
DUI_END_MESSAGE_MAP()

LPCTSTR CMainWnd::GetWindowClassName() const
{
	return _T("MainWnd");
}

CDuiString CMainWnd::GetSkinFolder()
{
	return _T("..//x86");
}

CDuiString CMainWnd::GetSkinFile()
{
	return _T("MainWnd.xml");
}

void CMainWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CMainWnd::OnClose(TNotifyUI& msg)
{
	Show_HideTask(FALSE);
	::PostQuitMessage(0L);
}

void CMainWnd::OnMin(TNotifyUI& msg)
{
	Show_HideTask(FALSE);
	m_IsMinWnd = TRUE;
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainWnd::OnDownLoadWnd(TNotifyUI& msg)
{
	DownLoadWnd* pDlg = new DownLoadWnd();
	assert(pDlg);
	pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_FRAME, 0L, 1024, 768, 0, 0);
	pDlg->CenterWindow();
	pDlg->ShowModal();
}

void CMainWnd::OnLogWnd(TNotifyUI& msg)
{
	CLogUI* pDlg = new CLogUI();
	assert(pDlg);
	pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_CONTAINER, 0L, 1024, 768, 0, 0);
	pDlg->CenterWindow();
	pDlg->ShowModal();
}

void CMainWnd::OnOtherToolsWnd(TNotifyUI& msg)
{
	COtherTools* pDlg = new COtherTools();
	assert(pDlg);
	pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_CONTAINER, 0L, 0, 0, 0, 0);
	pDlg->CenterWindow();
	pDlg->ShowModal();
}

LRESULT CMainWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_IsMinWnd)
	{
		Show_HideTask(TRUE);
	}
	m_IsMinWnd = FALSE;

	bHandled = FALSE;
	return FALSE;
}

void CMainWnd::Notify(TNotifyUI& msg)
{
	return WindowImplBase::NotifyPump(msg);
}


void CMainWnd::Show_HideTask(BOOL IsHide)
{
	int nCwdShow = -1;
	LPARAM lParam;
	HWND task = FindWindow(_T("Shell_TrayWnd"), NULL);
	if (IsHide)
	{
		lParam = ABS_AUTOHIDE | ABS_ALWAYSONTOP;
		nCwdShow = SW_HIDE;
	}
	else
	{
		lParam = ABS_ALWAYSONTOP;
		nCwdShow = SW_SHOW;
	}
	
	::ShowWindow(task, nCwdShow);

	APPBARDATA apBar;
	memset(&apBar, 0, sizeof(apBar));
	apBar.cbSize = sizeof(apBar);
	apBar.hWnd = task;
	if (apBar.hWnd != NULL)
	{
		apBar.lParam = lParam;
		SHAppBarMessage(ABM_SETSTATE, &apBar);
	}
}
