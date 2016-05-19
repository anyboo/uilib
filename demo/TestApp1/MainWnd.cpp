#include "StdAfx.h"
#include "MenuWnd.h"
#include "SettingWnd.h"
#include "MainWnd.h"

CMainWnd::CMainWnd()
{
	RegisterWindowClass();
}

CMainWnd::~CMainWnd()
{

}

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

DUI_BEGIN_MESSAGE_MAP(CMainWnd, CNotifyPump)
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
	DUITRACE(_T("Notify msg.sType : %s"), msg.sType);

	(msg.pSender) ? DUITRACE(_T("Notify msg.pSender : %s"), msg.pSender->GetName())
		: _T("unknown sender");
}

void CMainWnd::OnSettingDlg(TNotifyUI& msg)
{
	DUITRACE(_T("Notify msg.sType : %s"), msg.sType);

	(msg.pSender) ? DUITRACE(_T("Notify msg.pSender : %s"), msg.pSender->GetName())
		: _T("unknown sender");
}

void CMainWnd::OnMin(TNotifyUI& msg)
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainWnd::OnClose(TNotifyUI& msg)
{
	Close();
}

void CMainWnd::Notify(TNotifyUI& msg)
{
	return CNotifyPump::NotifyPump(msg);
}

LRESULT CMainWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("record.xml"), (UINT)0, NULL, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");

	CDialogBuilder builder2;
	CControlUI* pPage2 = builder2.Create(_T("recording.xml"), (UINT)0, NULL, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pPage2 = pPage2;

	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	return 0;
}

LRESULT CMainWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostQuitMessage(0L);

	return 0;
}

LRESULT CMainWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (::IsIconic(*this)) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CMainWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	RECT rcCaption = m_pm.GetCaptionRect();
	if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
		CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
		if (pControl && _tcscmp(pControl->GetClass(), DUI_CTR_BUTTON) != 0)
			return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CMainWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_pm.GetRoundCorner();
	if (!::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0)) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}

	bHandled = FALSE;
	return 0;
}


LRESULT CMainWnd::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);
	/*
	if ((pt.y < LYT_TOP) || (pt.y > LYT_BUTTON) || (pt.x < LYT_LEFT) || (pt.x >LYT_RIGHT))
		pt.x = 0;
	if (PAGE_RECORD == m_nPageState)
	{
		SetLytBkColor(_T("lytlz"), pt.x);
		SetLytBkColor(_T("lytPmbh"), pt.x);
		SetLytBkColor(_T("lytArea"), pt.x);
		SetLytBkColor(_T("lytOpen"), pt.x);
		SetLytBkColor(_T("lytCode"), pt.x);
		SetLytBkColor(_T("lytSound"), pt.x);
		SetLytBkColor(_T("lytAbout"), pt.x);
	}
	else
	{
		SetLytBkColor(_T("lytpause"), pt.x);
		SetLytBkColor(_T("lytstop"), pt.x);
		SetLytBkColor(_T("lytAbout"), pt.x);
	}
	*/
	return 0;
}

LRESULT CMainWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DUITRACEMSG(uMsg);
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_MOUSEMOVE:	   lRes = OnMouseMove(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if (bHandled) return lRes;
	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMainWnd::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CDuiString now("test");
	/*CLabelUI* TimeLabel = static_cast<CLabelUI*>(m_pm.FindControl(_T("LbTime")));
	TimeLabel->SetText(now);*/
	return false;
}
/*
void CMainWnd::SetPause()
{
	CButtonUI* cBtnPause = static_cast<CButtonUI*>(m_pm.FindControl(_T("btpause")));
	CLabelUI* cLbPause = static_cast<CLabelUI*>(m_pm.FindControl(_T("lbpause")));
	cBtnPause->SetNormalImage(_T("record/goon.png"));
	cLbPause->SetText(_T("继续"));
}

void CMainWnd::SetGoon()
{
	CButtonUI* cBtnPause = static_cast<CButtonUI*>(m_pm.FindControl(_T("btpause")));
	CLabelUI* cLbPause = static_cast<CLabelUI*>(m_pm.FindControl(_T("lbpause")));
	cBtnPause->SetNormalImage(_T("record/player_pause.png"));
	cLbPause->SetText(_T("暂停"));
}

bool CMainWnd::SetArea(int iAreaX, int iAreaY)
{
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);
	if ((cx < iAreaX) || (cy < iAreaY))
		return false;

	m_rArea.left = 0;
	m_rArea.right = iAreaX;
	m_rArea.top = 0;
	m_rArea.bottom = iAreaY;

	return true;
}

void CMainWnd::ChangePage()
{
	static int i = 0;
	if (0 == i)
		m_pPage1 = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("page1")));
	CHorizontalLayoutUI* cSelectPage = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("selectPage")));
	if (PAGE_RECORD == m_nPageState)
	{
		cSelectPage->Remove(m_pPage1, true);
		cSelectPage->Add(m_pPage2);
		m_nPageState = PAGE_RECORDING;
		SetTimer(m_hWnd, 1, 1000, TimerProc);

		//m_cRcdWk->OnRecord(m_bScreenRecord, m_bSoundRecord, m_rArea, m_iCode, m_bSysSound, m_bMcf, m_cSaveDir);
	}
	else
	{
		KillTimer(m_hWnd, 1);

		m_nRecordSecond = 0;
		CLabelUI* cLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("LbTime")));
		cLbl->SetText(_T("00:00:00"));

		cSelectPage->Remove(m_pPage2, true);
		cSelectPage->Add(m_pPage1);
		m_nPageState = PAGE_RECORD;
		if (0 != m_bPauseState % 2)
			SetGoon();
		//m_cRcdWk->OnStop(m_bScreenRecord, m_bSoundRecord, m_rArea, m_iCode, m_bSysSound, m_bMcf, m_cSaveDir);
		m_bPauseState = false;
	}
	i++;
}

void CMainWnd::ScreenToSound()
{
	if (STATE_RECORDSOUND == m_nRecordState)
		return;

	static int i = 0;
	if (0 == i)
	{
		m_JustScreenLyt[0] = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("lytPmbh")));
		m_JustScreenLyt[1] = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("lytArea")));
		m_JustScreenLyt[2] = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("lytOpen")));
		m_JustScreenLyt[3] = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("lytCode")));
		m_JustScreenLyt[4] = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("lytSound")));
		m_JustScreenLyt[5] = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("lytAbout")));
	}

	CVerticalLayoutUI* cLyt = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("lytWork")));
	cLyt->Remove(m_JustScreenLyt[0], true);
	cLyt->Remove(m_JustScreenLyt[1], true);

	m_bScreenRecord = false;
	m_bSoundRecord = true;
	m_nRecordState = STATE_RECORDSOUND;
	i++;
}

void CMainWnd::SoundToScreen()
{
	if (STATE_RECORDSCREEN == m_nRecordState)
		return;

	CVerticalLayoutUI* cLyt = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("lytWork")));
	cLyt->Add(m_JustScreenLyt[0]);
	cLyt->Add(m_JustScreenLyt[1]);

	cLyt->Remove(m_JustScreenLyt[2], true);
	cLyt->Add(m_JustScreenLyt[2]);

	cLyt->Remove(m_JustScreenLyt[3], true);
	cLyt->Add(m_JustScreenLyt[3]);

	cLyt->Remove(m_JustScreenLyt[4], true);
	cLyt->Add(m_JustScreenLyt[4]);

	cLyt->Remove(m_JustScreenLyt[5], true);
	cLyt->Add(m_JustScreenLyt[5]);

	m_bScreenRecord = true;
	m_bSoundRecord = true;
	m_nRecordState = STATE_RECORDSCREEN;
}

void CMainWnd::SetSaveDir()
{
	TCHAR szBuffer[MAX_PATH] = { 0 };
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = _T("设置存放目录:");
	bi.ulFlags = BIF_RETURNFSANCESTORS;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (NULL == idl)
	{
		return;
	}
	SHGetPathFromIDList(idl, szBuffer);
	memcpy(m_cSaveDir, szBuffer, MAX_PATH);
	if (PAGE_RECORDING == m_nPageState)
	{
		m_cRcdWk->OnSetChange(m_bScreenRecord, m_bSoundRecord, m_rArea, m_iCode, m_bSysSound, m_bMcf, m_cSaveDir);
	}

}

void CMainWnd::OpenRecordFile()
{
	HINSTANCE hIns = ShellExecute(NULL, _T("Open"), m_cSaveDir, NULL, NULL, SW_SHOWNORMAL);
	int dret = (int)hIns;
	if (dret < 32)
		MessageBox(NULL, _T("Open file Failure!"), _T("message"), MB_OK);
}


void CMainWnd::SaveSet()
{
	rapidjson::Document document;
	document.Parse("SaveSet.json");
	ofstream ofs("SaveSet.json");
	rapidjson::OStreamWrapper osw(ofs);

	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	rapidjson::Value file_path(rapidjson::kObjectType);
	rapidjson::Value root(rapidjson::kObjectType);

#ifndef UNICODE
	file_path.SetString(m_cSaveDir, allocator);
#else
	char cSaveData[MAX_PATH];
	memcpy(cSaveData, m_cSaveDir, MAX_PATH);
	file_path.SetString(cSaveData, allocator);
#endif
	root.AddMember("path", file_path, allocator);
	rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
	root.Accept(writer);
}

void CMainWnd::OpenSetPage()
{
	CSettingWnd* pRecordArea = new CSettingWnd(this);
	if (pRecordArea == NULL) { Close(); return; }
	pRecordArea->Create(NULL, _T("游戏中心"), UI_WNDSTYLE_FRAME, 0L, 0, 0, 1024, 738);
	pRecordArea->CenterWindow();
	::ShowWindow(*pRecordArea, SW_SHOW);

	CPaintManagerUI::MessageLoop();

	::CoUninitialize();

}
*/