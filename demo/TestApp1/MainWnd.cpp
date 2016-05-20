#include "StdAfx.h"
#include "MenuWnd.h"
#include "SettingWnd.h"
#include "AboutWnd.h"
#include "MainWnd.h"

#define CTR_RECORD			(_T("btlz"))
#define CTR_SCREEN_CAPTURE	(_T("btpmbh"))
#define CTR_AREA_RECORD		(_T("btlzqy"))
#define CTR_LOCATION		(_T("btopen"))
#define CTR_ENCODE			(_T("btencode"))
#define CTR_VOICE			(_T("btvoice"))
#define CTR_ABOUT			(_T("btabout"))
#define CTR_SETTING			(_T("savebtn"))
#define CTR_MIN				(_T("minbtn"))
#define CTR_CLOSE			(_T("closebtn"))

CMainWnd::CMainWnd()
{
}

CMainWnd::~CMainWnd()
{
}

DUI_BEGIN_MESSAGE_MAP(CMainWnd, WindowImplBase)
	DUI_ON_CLICK_CTRNAME(CTR_RECORD, OnRecord)
	DUI_ON_CLICK_CTRNAME(CTR_SCREEN_CAPTURE, OnScreenRecord)
	DUI_ON_CLICK_CTRNAME(CTR_AREA_RECORD, OnAreaRecord)
	DUI_ON_CLICK_CTRNAME(CTR_LOCATION, OnLocation)
	DUI_ON_CLICK_CTRNAME(CTR_ENCODE, OnEncode)
	DUI_ON_CLICK_CTRNAME(CTR_VOICE, OnVoice)
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

void CMainWnd::OnRecord(TNotifyUI& msg)
{
	DUITRACE(_T("Notify msg.sType : %s"), msg.sType);

	(msg.pSender) ? DUITRACE(_T("Notify msg.pSender : %s"), msg.pSender->GetName()) : _T("unknown sender");
}

void CMainWnd::OnScreenRecord(TNotifyUI& msg)
{
	DUITRACE(_T("Notify msg.sType : %s"), msg.sType);

	(msg.pSender) ? DUITRACE(_T("Notify msg.pSender : %s"), msg.pSender->GetName()) 
		: _T("unknown sender");
}

void CMainWnd::OnAreaRecord(TNotifyUI& msg)
{
	DUITRACE(_T("Notify msg.sType : %s"), msg.sType);

	(msg.pSender) ? DUITRACE(_T("Notify msg.pSender : %s"), msg.pSender->GetName())
		: _T("unknown sender");
}

void CMainWnd::OnLocation(TNotifyUI& msg)
{
	DUITRACE(_T("Notify msg.sType : %s"), msg.sType);

	(msg.pSender) ? DUITRACE(_T("Notify msg.pSender : %s"), msg.pSender->GetName())
		: _T("unknown sender");
}

void CMainWnd::OnEncode(TNotifyUI& msg)
{
	DUITRACE(_T("Notify msg.sType : %s"), msg.sType);

	(msg.pSender) ? DUITRACE(_T("Notify msg.pSender : %s"), msg.pSender->GetName())
		: _T("unknown sender");
}

void CMainWnd::OnVoice(TNotifyUI& msg)
{
	DUITRACE(_T("Notify msg.sType : %s"), msg.sType);

	(msg.pSender) ? DUITRACE(_T("Notify msg.pSender : %s"), msg.pSender->GetName())
		: _T("unknown sender");
}

void CMainWnd::OnAbout(TNotifyUI& msg)
{
	CAboutWnd* pDlg = new CAboutWnd();
	assert(pDlg);
	pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_DIALOG, 0L, 0, 0, 500, 400);
	pDlg->CenterWindow();
	pDlg->ShowModal();
}

void CMainWnd::OnSettingDlg(TNotifyUI& msg)
{
	CSettingWnd* pSettingDlg = new CSettingWnd();
	assert(pSettingDlg);
	pSettingDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_DIALOG, 0L, 0, 0, 500, 400);
	pSettingDlg->CenterWindow();
	pSettingDlg->ShowModal();
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
	return WindowImplBase::NotifyPump(msg);
}

void CMainWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}
