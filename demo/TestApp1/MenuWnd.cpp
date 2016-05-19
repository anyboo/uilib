#include "stdafx.h"
#include <tchar.h>
#include "MenuWnd.h"
#include "RecordWork.h"


inline HBITMAP CreateMyBitmap(HDC hDC, int cx, int cy, COLORREF** pBits)
{
	LPBITMAPINFO lpbiSrc = NULL;
	lpbiSrc = (LPBITMAPINFO) new BYTE[sizeof(BITMAPINFOHEADER)];
	if (lpbiSrc == NULL) return NULL;

	lpbiSrc->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbiSrc->bmiHeader.biWidth = cx;
	lpbiSrc->bmiHeader.biHeight = cy;
	lpbiSrc->bmiHeader.biPlanes = 1;
	lpbiSrc->bmiHeader.biBitCount = 32;
	lpbiSrc->bmiHeader.biCompression = BI_RGB;
	lpbiSrc->bmiHeader.biSizeImage = cx * cy;
	lpbiSrc->bmiHeader.biXPelsPerMeter = 0;
	lpbiSrc->bmiHeader.biYPelsPerMeter = 0;
	lpbiSrc->bmiHeader.biClrUsed = 0;
	lpbiSrc->bmiHeader.biClrImportant = 0;

	HBITMAP hBitmap = CreateDIBSection(hDC, lpbiSrc, DIB_RGB_COLORS, (void **)pBits, NULL, NULL);
	delete[] lpbiSrc;
	return hBitmap;
}

CShadowWnd::CShadowWnd() 
{
	m_di.sDrawString = _T("file='menu_bk.png' corner='40,8,8,8'");
}

LPCTSTR CShadowWnd::GetWindowClassName() const 
{ 
	return _T("UIShadow");
}

UINT CShadowWnd::GetClassStyle() const 
{
	return UI_CLASSSTYLE_FRAME;
}

void CShadowWnd::OnFinalMessage(HWND /*hWnd*/) 
{ 
	delete this; 
};

void CShadowWnd::RePaint()
{
	RECT rcClient = { 0 };
	::GetClientRect(m_hWnd, &rcClient);
	DWORD dwWidth = rcClient.right - rcClient.left;
	DWORD dwHeight = rcClient.bottom - rcClient.top;

	HDC hDcPaint = ::GetDC(m_hWnd);
	HDC hDcBackground = ::CreateCompatibleDC(hDcPaint);
	COLORREF* pBackgroundBits;
	HBITMAP hbmpBackground = CreateMyBitmap(hDcPaint, dwWidth, dwHeight, &pBackgroundBits);
	::ZeroMemory(pBackgroundBits, dwWidth * dwHeight * 4);
	HBITMAP hOldBitmap = (HBITMAP) ::SelectObject(hDcBackground, hbmpBackground);

	CRenderEngine::DrawImage(hDcBackground, &m_pm, rcClient, rcClient, m_di);

	RECT rcWnd = { 0 };
	::GetWindowRect(m_hWnd, &rcWnd);

	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	POINT ptPos = { rcWnd.left, rcWnd.top };
	SIZE sizeWnd = { dwWidth, dwHeight };
	POINT ptSrc = { 0, 0 };
	UpdateLayeredWindow(m_hWnd, hDcPaint, &ptPos, &sizeWnd, hDcBackground, &ptSrc, 0, &bf, ULW_ALPHA);

	::SelectObject(hDcBackground, hOldBitmap);
	if (hDcBackground != NULL) ::DeleteDC(hDcBackground);
	if (hbmpBackground != NULL) ::DeleteObject(hbmpBackground);
	::ReleaseDC(m_hWnd, hDcPaint);

	m_bNeedUpdate = false;
}

LRESULT CShadowWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE) SetTimer(m_hWnd, 9000, 10, NULL);
	else if (uMsg == WM_SIZE) m_bNeedUpdate = true;
	else if (uMsg == WM_CLOSE) KillTimer(m_hWnd, 9000);
	else if (uMsg == WM_TIMER) {
		if (LOWORD(wParam) == 9000 && m_bNeedUpdate == true) {
			if (!::IsIconic(m_hWnd)) RePaint();
		}
	}

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}


CMenuWnd::CMenuWnd()
:m_pOwner(nullptr),
m_pShadowWnd(nullptr)
{

}

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
	/*
	if (msg.sType == _T("windowinit")) OnPrepare();
	if (msg.sType == _T("click")) {
		bool bClose = true;
		//record model
		if (msg.pSender->GetName() == _T("btnSound")) {
			m_Record->m_pLzPageName = _T("LbSound");
			m_Record->ScreenToSound();
		}
		if (msg.pSender->GetName() == _T("btnScreen")) {
			m_Record->m_pLzPageName = _T("LbScreen");
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
			m_Record->m_pCodeName = _T("LbGif");
		}
		if (msg.pSender->GetName() == _T("BtnMP4")) {
			m_Record->m_pCodeName = _T("LbMp4");
			m_Record->m_iCode = CODE_MP4;
		}
		if (msg.pSender->GetName() == _T("BtnMkv")) {
			m_Record->m_iCode = CODE_MKV;
			m_Record->m_pCodeName = _T("LbMkv");
		}
		if (msg.pSender->GetName() == _T("BtnM4v")) {
			m_Record->m_iCode = CODE_M4V;
			m_Record->m_pCodeName = _T("LbM4v");
		}
		if (msg.pSender->GetName() == _T("BtnMov")) {
			m_Record->m_iCode = CODE_MOV;
			m_Record->m_pCodeName = _T("LbMov");
		}
		if (msg.pSender->GetName() == _T("BtnWmv")) {
			m_Record->m_iCode = CODE_WMV;
			m_Record->m_pCodeName = _T("LbWmv");
		}
		if (msg.pSender->GetName() == _T("BtnFlv")) {
			m_Record->m_iCode = CODE_FLV;
			m_Record->m_pCodeName = _T("LbFlv");
		}
		if (msg.pSender->GetName() == _T("BtnAvi")) {
			m_Record->m_iCode = CODE_AVI;
			m_Record->m_pCodeName = _T("LbAvi");
		}
		if (msg.pSender->GetName() == _T("BtnTs")) {
			m_Record->m_iCode = CODE_TS;
			m_Record->m_pCodeName = _T("LbTs");
		}
		if (msg.pSender->GetName() == _T("BtnVob")) {
			m_Record->m_iCode = CODE_VOB;
			m_Record->m_pCodeName = _T("LbVob");
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
	*/
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


void CMenuWnd::ChooseCode(LPCTSTR pLbName)
{
	SetChooseCode(pLbName, _T("LbGif"));
	SetChooseCode(pLbName, _T("LbMp4"));
	SetChooseCode(pLbName, _T("LbMkv"));
	SetChooseCode(pLbName, _T("LbM4v"));
	SetChooseCode(pLbName, _T("LbMov"));
	SetChooseCode(pLbName, _T("LbWmv"));
	SetChooseCode(pLbName, _T("LbFlv"));
	SetChooseCode(pLbName, _T("LbAvi"));
	SetChooseCode(pLbName, _T("LbTs"));
	SetChooseCode(pLbName, _T("LbVob"));
}

void CMenuWnd::ChooseLzPage(LPCTSTR pLbName)
{
	SetChooseCode(pLbName, _T("LbScreen"));
	SetChooseCode(pLbName, _T("LbSound"));
}

void CMenuWnd::SetChooseCode(LPCTSTR pDesLbName, LPCTSTR pLbName)
{
	CLabelUI* cLb = static_cast<CLabelUI*>(m_pm.FindControl(pLbName));
	if (NULL == cLb)
		return;
	if (!strcmp(pDesLbName, pLbName))
	{
		cLb->SetBkImage(_T("record/choose.png"));
	}
	else
	{
		cLb->SetBkImage(_T(""));
	}
}


void CMenuWnd::OnPrepare()
{
	/*if (NULL != m_Record->m_pCodeName)
		ChooseCode(m_Record->m_pCodeName);
	if (NULL != m_Record->m_pLzPageName)
		ChooseLzPage(m_Record->m_pLzPageName);
	if (m_Record->m_bSysSound)
	{
		CLabelUI* cLb = static_cast<CLabelUI*>(m_pm.FindControl(_T("LbSysSound")));
		cLb->SetBkImage(_T("record/choose.png"));
	}

	CLabelUI* cLbMcf = static_cast<CLabelUI*>(m_pm.FindControl(_T("LbMcf")));
	if (NULL == cLbMcf)
		return;
	CLabelUI* cLbUnMcf = static_cast<CLabelUI*>(m_pm.FindControl(_T("LbUnMcf")));
	if (NULL == cLbUnMcf)
		return;
	if (m_Record->m_bMcf)
	{
		cLbMcf->SetBkImage(_T("record/choose.png"));
		cLbUnMcf->SetBkImage(_T(""));
	}
	else
	{
		cLbUnMcf->SetBkImage(_T("record/choose.png"));
		cLbMcf->SetBkImage(_T(""));
	}*/
}
