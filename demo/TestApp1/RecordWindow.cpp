#include "stdafx.h"
#include "RecordWindow.h"

CRecordWindow::CRecordWindow(CRecord* cRcd)
{
	m_cRcd = cRcd;
}


CRecordWindow::~CRecordWindow()
{
}

LPCTSTR CRecordWindow::GetWindowClassName() const
{
	return _T("MenuWnd");
}

UINT CRecordWindow::GetClassStyle() const 
{ 
	return UI_CLASSSTYLE_DIALOG; 
}

void CRecordWindow::OnFinalMessage(HWND /*hWnd*/)
{
	m_pm.RemovePreMessageFilter(this);
	delete this;
}

void CRecordWindow::Init() {
	CComboUI* pAccountCombo = static_cast<CComboUI*>(m_pm.FindControl(_T("accountcombo")));
	CEditUI* pAccountEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("accountedit")));
	if (pAccountCombo && pAccountEdit) pAccountEdit->SetText(pAccountCombo->GetText());
	pAccountEdit->SetFocus();
}

void CRecordWindow::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("windowinit")) OnPrepare();
	if (msg.sType == _T("click")) {
		if (msg.pSender->GetName() == _T("closebtn")) { Close(); return; }
		else if (msg.pSender->GetName() == _T("loginBtn")) { Close(); return; }
		else if (msg.pSender->GetName() == _T("btnSetDir")) 
		{ 
			m_cRcd->SetSaveDir();
			CEditUI* cEui = static_cast<CEditUI*>(m_pm.FindControl(_T("EditDir")));
			cEui->SetText(m_cRcd->m_cSaveDir);
			return; 
		}
		else if (msg.pSender->GetName() == _T("opgif"))
		{
			m_cRcd->m_iCode = CODE_GIF;
			m_cRcd->m_pCodeName = _T("LbGif");
		}
		if (msg.pSender->GetName() == _T("opmp4")) {
			m_cRcd->m_pCodeName = _T("LbMp4");
			m_cRcd->m_iCode = CODE_MP4;
		}
		if (msg.pSender->GetName() == _T("opmkv")) {
			m_cRcd->m_iCode = CODE_MKV;
			m_cRcd->m_pCodeName = _T("LbMkv");
		}
		if (msg.pSender->GetName() == _T("opm4v")) {
			m_cRcd->m_iCode = CODE_M4V;
			m_cRcd->m_pCodeName = _T("LbM4v");
		}
		if (msg.pSender->GetName() == _T("opmov")) {
			m_cRcd->m_iCode = CODE_MOV;
			m_cRcd->m_pCodeName = _T("LbMov");
		}
		if (msg.pSender->GetName() == _T("opwmv")) {
			m_cRcd->m_iCode = CODE_WMV;
			m_cRcd->m_pCodeName = _T("LbWmv");
		}
		if (msg.pSender->GetName() == _T("opflv")) {
			m_cRcd->m_iCode = CODE_FLV;
			m_cRcd->m_pCodeName = _T("LbFlv");
		}
		if (msg.pSender->GetName() == _T("opavi")) {
			m_cRcd->m_iCode = CODE_AVI;
			m_cRcd->m_pCodeName = _T("LbAvi");
		}
		if (msg.pSender->GetName() == _T("opts")) {
			m_cRcd->m_iCode = CODE_TS;
			m_cRcd->m_pCodeName = _T("LbTs");
		}
		if (msg.pSender->GetName() == _T("opvob")) {
			m_cRcd->m_iCode = CODE_VOB;
			m_cRcd->m_pCodeName = _T("LbVob");
		}
		if (PAGE_RECORDING == m_cRcd->m_nPageState)
		{
			m_cRcd->m_cRcdWk->OnSetChange(m_cRcd->m_bScreenRecord, m_cRcd->m_bSoundRecord,
				m_cRcd->m_rArea, m_cRcd->m_iCode, m_cRcd->m_bSysSound, m_cRcd->m_bMcf, m_cRcd->m_cSaveDir);
		}
	}
}

LRESULT CRecordWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);

	m_pm.Init(m_hWnd);
	m_pm.AddPreMessageFilter(this);
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("set.xml"), (UINT)0, NULL, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	Init();
	return 0;
}

LRESULT CRecordWindow::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CRecordWindow::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CRecordWindow::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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


LRESULT CRecordWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
//	case WM_MOUSEMOVE:	   OnMouseMove(uMsg, wParam, lParam, bHandled); break;
//	case MOUSEEVENTF_LEFTDOWN:	OnMouseMove(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if (bHandled) return lRes;
	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CRecordWindow::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled)
{
	if (uMsg == WM_KEYDOWN) {
		if (wParam == VK_RETURN) {
			CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("accountedit")));
			if (pEdit->GetText().IsEmpty()) pEdit->SetFocus();
			else {
				pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("pwdedit")));
				if (pEdit->GetText().IsEmpty()) pEdit->SetFocus();
				else Close();
			}
			return true;
		}
		else if (wParam == VK_ESCAPE) {
			PostQuitMessage(0);
			return true;
		}

	}
	return false;
}

void CRecordWindow::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);

	CEditUI* cLytV = static_cast<CEditUI*>(m_pm.FindControl(_T("accountedit")));
	if ((pt.x < cLytV->GetPos().right) && (pt.x > cLytV->GetPos().left) && (pt.y < cLytV->GetPos().bottom) && (pt.y > cLytV->GetPos().top))
	{
		cLytV->SetBkColor(LYT_BK_COLOR);
	}

}

void CRecordWindow::OnPrepare()
{
	COptionUI* pOp = NULL;

	if (m_cRcd->m_iCode == CODE_GIF)
		pOp = static_cast<COptionUI*>(m_pm.FindControl(_T("opgif")));
	if (m_cRcd->m_iCode == CODE_MP4)
		pOp = static_cast<COptionUI*>(m_pm.FindControl(_T("opMp4")));
	if (m_cRcd->m_iCode == CODE_MKV)
		pOp = static_cast<COptionUI*>(m_pm.FindControl(_T("opmkv")));
	if (m_cRcd->m_iCode == CODE_M4V)
		pOp = static_cast<COptionUI*>(m_pm.FindControl(_T("opm4v")));
	if (m_cRcd->m_iCode == CODE_MOV)
		pOp = static_cast<COptionUI*>(m_pm.FindControl(_T("opmov")));
	if (m_cRcd->m_iCode == CODE_WMV)
		pOp = static_cast<COptionUI*>(m_pm.FindControl(_T("opwmv")));
	if (m_cRcd->m_iCode == CODE_FLV)
		pOp = static_cast<COptionUI*>(m_pm.FindControl(_T("opflv")));
	if (m_cRcd->m_iCode == CODE_AVI)
		pOp = static_cast<COptionUI*>(m_pm.FindControl(_T("opavi")));
	if (m_cRcd->m_iCode == CODE_TS)
		pOp = static_cast<COptionUI*>(m_pm.FindControl(_T("opts")));
	if (m_cRcd->m_iCode == CODE_VOB)
		pOp = static_cast<COptionUI*>(m_pm.FindControl(_T("opvob")));
	if (NULL != pOp)
		pOp->Selected(true);

	CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(_T("EditDir")));
	pEdit->SetText(m_cRcd->m_cSaveDir);
}






