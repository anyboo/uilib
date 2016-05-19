#include "stdafx.h"
#include "SettingWnd.h"

CSettingWnd::CSettingWnd()
{
	RegisterWindowClass();
	pm = CPaintManagerUI::GetPaintManager(GetWindowClassName());
	assert(pm);
}

CSettingWnd::~CSettingWnd()
{
}

LPCTSTR CSettingWnd::GetWindowClassName() const
{
	return _T("SettingWnd");
}

void CSettingWnd::OnFinalMessage(HWND /*hWnd*/)
{
	delete this;
}

void CSettingWnd::Init() 
{
	assert(pm);
	CComboUI* pAccountCombo = static_cast<CComboUI*>(pm->FindControl(_T("accountcombo")));
	CEditUI* pAccountEdit = static_cast<CEditUI*>(pm->FindControl(_T("accountedit")));
	if (pAccountCombo && pAccountEdit) pAccountEdit->SetText(pAccountCombo->GetText());
	pAccountEdit->SetFocus();
}

void CSettingWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("windowinit")) 
		OnPrepare();
	if (msg.sType == _T("click")) {
		if (msg.pSender->GetName() == _T("closebtn")) { Close(); return; }
		else if (msg.pSender->GetName() == _T("loginBtn")) { Close(); return; }
		else if (msg.pSender->GetName() == _T("btnSetDir")) 
		{ 
		
			/*CEditUI* cEui = static_cast<CEditUI*>(m_pm.FindControl(_T("EditDir")));
			cEui->SetText(m_cRcd->m_cSaveDir);*/
			return; 
		}
		else if (msg.pSender->GetName() == _T("opgif"))
		{

		}
		if (msg.pSender->GetName() == _T("opmp4")) {
			
		}
		if (msg.pSender->GetName() == _T("opmkv")) {
			
		}
		if (msg.pSender->GetName() == _T("opm4v")) {
			
		}
		if (msg.pSender->GetName() == _T("opmov")) {
		
		}
		if (msg.pSender->GetName() == _T("opwmv")) {
		
		}
		if (msg.pSender->GetName() == _T("opflv")) {
		
		}
		if (msg.pSender->GetName() == _T("opavi")) {
			
		}
		if (msg.pSender->GetName() == _T("opts")) {
			
		}
		if (msg.pSender->GetName() == _T("opvob")) {
			
		}
	}
}

LRESULT CSettingWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	assert(pm);
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	pm->Init(m_hWnd);
	//pm->AddPreMessageFilter(this);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("set.xml"), (UINT)0, NULL, pm);
	ASSERT(pRoot && "Failed to parse XML");
	pm->AttachDialog(pRoot);
	pm->AddNotifier(this);

	Init();
	return 0;
}

LRESULT CSettingWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	assert(pm);
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	RECT rcCaption = pm->GetCaptionRect();
	if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
		CControlUI* pControl = static_cast<CControlUI*>(pm->FindControl(pt));
		if (pControl && _tcscmp(pControl->GetClass(), DUI_CTR_BUTTON) != 0)
			return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CSettingWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	assert(pm);
	SIZE szRoundCorner = pm->GetRoundCorner();
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

LRESULT CSettingWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	CDuiRect rcWork = oMonitor.rcWork;
	LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
	lpMMI->ptMaxPosition.x = rcWork.left;
	lpMMI->ptMaxPosition.y = rcWork.top;
	lpMMI->ptMaxSize.x = rcWork.right - rcWork.left;
	lpMMI->ptMaxSize.y = rcWork.bottom - rcWork.top;
	bHandled = FALSE;
	return 0;
}


LRESULT CSettingWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	assert(pm);
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
//	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	//case WM_MOUSEMOVE:	   lRes = OnMouseMove(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if (bHandled) return lRes;
	if (pm->MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}


void CSettingWnd::OnPrepare()
{
	COptionUI* pOp = NULL;

	//if (m_cRcd->m_iCode == CODE_GIF)
	//	pOp = static_cast<COptionUI*>(m_pm.FindControl(_T("opgif")));
	//if (m_cRcd->m_iCode == CODE_MP4)
	//	pOp = static_cast<COptionUI*>(m_pm.FindControl(_T("opMp4")));
	//if (m_cRcd->m_iCode == CODE_MKV)
	//	pOp = static_cast<COptionUI*>(m_pm.FindControl(_T("opmkv")));
	//if (m_cRcd->m_iCode == CODE_M4V)
	//	pOp = static_cast<COptionUI*>(m_pm.FindControl(_T("opm4v")));
	//if (m_cRcd->m_iCode == CODE_MOV)
	//	pOp = static_cast<COptionUI*>(m_pm.FindControl(_T("opmov")));
	//if (m_cRcd->m_iCode == CODE_WMV)
	//	pOp = static_cast<COptionUI*>(m_pm.FindControl(_T("opwmv")));
	//if (m_cRcd->m_iCode == CODE_FLV)
	//	pOp = static_cast<COptionUI*>(m_pm.FindControl(_T("opflv")));
	//if (m_cRcd->m_iCode == CODE_AVI)
	//	pOp = static_cast<COptionUI*>(m_pm.FindControl(_T("opavi")));
	//if (m_cRcd->m_iCode == CODE_TS)
	//	pOp = static_cast<COptionUI*>(m_pm.FindControl(_T("opts")));
	//if (m_cRcd->m_iCode == CODE_VOB)
	//	pOp = static_cast<COptionUI*>(m_pm.FindControl(_T("opvob")));
	if (NULL != pOp)
		pOp->Selected(true);

	/*CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("EditDir")));
	pEdit->SetText(m_cRcd->m_cSaveDir);*/
}






