#include "stdafx.h"
#include "MenuWnd.h"
#include "RecordWork.h"

void CMenuWnd::Init(CControlUI* pOwner, POINT pt) {
	if (pOwner == NULL) return;
	m_pOwner = pOwner;
	m_ptPos = pt;
	Create(pOwner->GetManager()->GetPaintWindow(), NULL, WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_TOOLWINDOW);
	HWND hWndParent = m_hWnd;
	while (::GetParent(hWndParent) != NULL) hWndParent = ::GetParent(hWndParent);
	::ShowWindow(m_hWnd, SW_SHOW);
	::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
}

void CMenuWnd::AdjustPostion() {
	CDuiRect rcWnd;
	GetWindowRect(m_hWnd, &rcWnd);
	int nWidth = rcWnd.GetWidth();
	int nHeight = rcWnd.GetHeight();
	rcWnd.left = m_ptPos.x;
	rcWnd.top = m_ptPos.y;
	rcWnd.right = rcWnd.left + nWidth;
	rcWnd.bottom = rcWnd.top + nHeight;
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;

	if (rcWnd.bottom > rcWork.bottom) {
		if (nHeight >= rcWork.GetHeight()) {
			rcWnd.top = 0;
			rcWnd.bottom = nHeight;
		}
		else {
			rcWnd.bottom = rcWork.bottom;
			rcWnd.top = rcWnd.bottom - nHeight;
		}
	}
	if (rcWnd.right > rcWork.right) {
		if (nWidth >= rcWork.GetWidth()) {
			rcWnd.left = 0;
			rcWnd.right = nWidth;
		}
		else {
			rcWnd.right = rcWork.right;
			rcWnd.left = rcWnd.right - nWidth;
		}
	}
	::SetWindowPos(m_hWnd, NULL, m_ptPos.x, m_ptPos.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

LPCTSTR CMenuWnd::GetWindowClassName() const 
{ 
	return _T("MenuWnd"); 
}

void CMenuWnd::OnFinalMessage(HWND /*hWnd*/)
{ 
	delete this; 
}

void CMenuWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click")) {
		bool bClose = true;
		//record model
		if (msg.pSender->GetName() == _T("LbSound")) {
			m_Record->ScreenToSound();
		}
		if (msg.pSender->GetName() == _T("LbScreen")) {
			m_Record->SoundToScreen();
		}

		//sound model
		if (msg.pSender->GetName() == _T("btnSysSound")) {
			if (true == m_Record->m_bSysSound)
				m_Record->m_bSysSound = false;
			else
				m_Record->m_bSysSound = true;
		}
		if (msg.pSender->GetName() == _T("btnMcf")) {
			m_Record->m_bMcf = true;
		}
		if (msg.pSender->GetName() == _T("btnUnmcf")) {
			m_Record->m_bMcf = false;
		}

		//Code model
		if (msg.pSender->GetName() == _T("BtnGif")) {
			m_Record->m_iCode = CODE_GIF;
		}
		if (msg.pSender->GetName() == _T("BtnMP4")) {
			m_Record->m_iCode = CODE_MP4;
		}
		if (msg.pSender->GetName() == _T("BtnMkv")) {
			m_Record->m_iCode = CODE_MKV;
		}
		if (msg.pSender->GetName() == _T("BtnM4v")) {
			m_Record->m_iCode = CODE_M4V;
		}
		if (msg.pSender->GetName() == _T("BtnMov")) {
			m_Record->m_iCode = CODE_MOV;
		}
		if (msg.pSender->GetName() == _T("BtnWmv")) {
			m_Record->m_iCode = CODE_WMV;
		}
		if (msg.pSender->GetName() == _T("BtnFlv")) {
			m_Record->m_iCode = CODE_FLV;
		}
		if (msg.pSender->GetName() == _T("BtnAvi")) {
			m_Record->m_iCode = CODE_AVI;
		}
		if (msg.pSender->GetName() == _T("BtnTs")) {
			m_Record->m_iCode = CODE_TS;
		}
		if (msg.pSender->GetName() == _T("BtnVob")) {
			m_Record->m_iCode = CODE_VOB;
		}

		//area model
		if (msg.pSender->GetName() == _T("btn_640_360")) {
			bClose = m_Record->SetArea(640, 360);
		}
		if (msg.pSender->GetName() == _T("btn_800_450")) {
			bClose = m_Record->SetArea(800, 450);
		}
		if (msg.pSender->GetName() == _T("btn_1024_576")) {
			bClose = m_Record->SetArea(1024, 576);
		}
		if (msg.pSender->GetName() == _T("btn_1280_720")) {
			bClose = m_Record->SetArea(1280, 720);
		}
		if (msg.pSender->GetName() == _T("btn_800_600")) {
			bClose = m_Record->SetArea(800, 600);
		}
		if (msg.pSender->GetName() == _T("btn_1024_768")) {
			bClose = m_Record->SetArea(1024, 768);
		}
		if (msg.pSender->GetName() == _T("btn_1920_1080")) {
			bClose = m_Record->SetArea(1920, 1080);
		}
		if (msg.pSender->GetName() == _T("btn_2560_1440")) {
			bClose = m_Record->SetArea(2560, 1440);
		}
		if (msg.pSender->GetName() == _T("btn_3840_2160")) {
			bClose = m_Record->SetArea(3840, 2160);
		}
		if (msg.pSender->GetName() == _T("btn_7680_4320")) {
			bClose = m_Record->SetArea(7680, 4320);
		}
		if (PAGE_RECORDING == m_Record->m_nPageState)
		{
			m_Record->m_cRcdWk->OnSetChange(m_Record->m_bScreenRecord, m_Record->m_bSoundRecord, 
				m_Record->m_rArea, m_Record->m_iCode, m_Record->m_bSysSound, m_Record->m_bMcf, m_Record->m_cSaveDir);
		}
		Close();
	}
	if (msg.sType == _T("itemselect")) {
		Close();
	}
	else if (msg.sType == _T("itemclick")) {
		if (msg.pSender->GetName() == _T("menu_Delete")) {
			if (m_pOwner) m_pOwner->GetManager()->SendNotify(m_pOwner, _T("menu_Delete"), 0, 0, true);
		}
	}
}

HWND CMenuWnd::Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x, int y, int cx, int cy, HMENU hMenu)
{
	if (m_pShadowWnd == NULL) m_pShadowWnd = new CShadowWnd;
	m_pShadowWnd->Create(hwndParent, _T(""), WS_VISIBLE | WS_POPUP | WS_CLIPSIBLINGS, WS_EX_LAYERED | WS_EX_TOOLWINDOW, x, y, cx, cy, NULL);

	dwExStyle |= WS_EX_TOOLWINDOW;
	return CWindowWnd::Create(hwndParent, pstrName, dwStyle, dwExStyle, x, y, cx, cy, hMenu);
}

void CMenuWnd::ShowWindow(bool bShow = true, bool bTakeFocus = true)
{
	if (m_pShadowWnd != NULL) m_pShadowWnd->ShowWindow(bShow, false);
	CWindowWnd::ShowWindow(bShow, bTakeFocus);
}

LRESULT CMenuWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_pm.Init(m_hWnd);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(m_pFileName, (UINT)0, NULL, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);
	m_pm.SetRoundCorner(3, 3);
	AdjustPostion();
	return 0;
}

LRESULT CMenuWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_pShadowWnd != NULL) m_pShadowWnd->Close();
	bHandled = FALSE;
	return 0;
}

LRESULT CMenuWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if ((HWND)wParam == m_hWnd) bHandled = TRUE;
	else if (m_pShadowWnd != NULL && (HWND)wParam == m_pShadowWnd->GetHWND()) {
		CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		::SetFocus(m_hWnd);
		bHandled = TRUE;
	}
	else {
		Close();
		bHandled = FALSE;
	}
	return 0;
}

LRESULT CMenuWnd::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == VK_ESCAPE) Close();
	return 0;
}

LRESULT CMenuWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (!::IsIconic(*this)) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		HRGN hRgn = ::CreateRectRgn(rcWnd.left + 8, rcWnd.top + 8, rcWnd.right - 8, rcWnd.bottom - 8);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}
	if (m_pShadowWnd != NULL) {
		RECT rcWnd = { 0 };
		::GetWindowRect(m_hWnd, &rcWnd);
		::SetWindowPos(*m_pShadowWnd, m_hWnd, rcWnd.left, rcWnd.top, rcWnd.right - rcWnd.left,
			rcWnd.bottom - rcWnd.top, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CMenuWnd::OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (m_pShadowWnd != NULL) {
		RECT rcWnd = { 0 };
		::GetWindowRect(m_hWnd, &rcWnd);
		::SetWindowPos(*m_pShadowWnd, m_hWnd, rcWnd.left, rcWnd.top, rcWnd.right - rcWnd.left,
			rcWnd.bottom - rcWnd.top, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CMenuWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_KILLFOCUS:     lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break;
	case WM_KEYDOWN:       lRes = OnKeyDown(uMsg, wParam, lParam, bHandled); break;
	case WM_MOUSEWHEEL:    break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_MOVE:          lRes = OnMove(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if (bHandled) return lRes;
	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}