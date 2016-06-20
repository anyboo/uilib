#include "stdafx.h"
#include "MainWnd.h"
#include "DownLoadWnd.h"
#include "LogUI.h"
#include "VideoLoginUI.h"

CMainWnd::CMainWnd()
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
	::PostQuitMessage(0L);
}

void CMainWnd::OnMin(TNotifyUI& msg)
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainWnd::OnDownLoadWnd(TNotifyUI& msg)
{
	DownLoadWnd* pDlg = new DownLoadWnd();
	assert(pDlg);
	pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_DIALOG, 0L, 0, 0, 1024, 768);
	pDlg->CenterWindow();
	pDlg->ShowModal();
}

void CMainWnd::OnLogWnd(TNotifyUI& msg)
{
	CLogUI* pDlg = new CLogUI();
	assert(pDlg);
	pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_DIALOG, 0L, 0, 0, 1024, 768);
	pDlg->CenterWindow();
	pDlg->ShowModal();
}

//void CMainWnd::OnVideoLonginWnd(TNotifyUI& msg)
//{
//	VideoLoginUI* pDlg = new VideoLoginUI();
//	assert(pDlg);
//	pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_DIALOG, 0L, 0, 0, 1024, 768);
//	pDlg->CenterWindow();
//	pDlg->ShowModal();
//}

void CMainWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)	{
		if (msg.pSender->GetName() == BT_VideoPlay)
		{
			
		}
	}
	return WindowImplBase::NotifyPump(msg);
}
