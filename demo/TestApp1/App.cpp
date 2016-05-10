// App.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "record.h"
#include "resource.h"
#include "MenuWnd.h"
#include <commdlg.h>
#include <shlobj.h>
#include "RecordWork.h"
using namespace std;

void CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT nTimerid, DWORD dwTime)
{
	SendMessage(hWnd, WM_TIMER, NULL, NULL);
}


wstring UTF8ToUnicode( const string& szSrcText )
{
	int  len = 0;
	len = szSrcText.length();
	int  unicodeLen = ::MultiByteToWideChar( CP_UTF8,
	0,
	szSrcText.c_str(),
	-1,
	NULL,
	0 );  
	wchar_t *  pUnicode;  
	pUnicode = new  wchar_t[unicodeLen+1];  
	memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t));  
	::MultiByteToWideChar( CP_UTF8,
	0,
	szSrcText.c_str(),
	-1,
	(LPWSTR)pUnicode,
	unicodeLen );  
	wstring  rt;
	rt = (wchar_t*)pUnicode;
	delete  pUnicode;
	return  rt;
}




CRecord::CRecord() {};

LPCTSTR CRecord::GetWindowClassName() const 
{ 
	return _T("UIRecord"); 
}

UINT CRecord::GetClassStyle() const 
{ 
	return UI_CLASSSTYLE_DIALOG; 
}

void CRecord::OnFinalMessage(HWND /*hWnd*/) 
{ 
	delete this;
}

void CRecord::Init()
{
}

void CRecord::Notify(TNotifyUI& msg)
{
	CRecordWork cRcdWk;
	if (msg.sType == _T("click")) {
		if (msg.pSender->GetName() == _T("closebtn")) {
			PostQuitMessage(0);
			return;
		}
		if (msg.pSender->GetName() == _T("minbtn")) {
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
			return;
		}
		if (msg.pSender->GetName() == _T("savebtn")) {
			SetSaveDir();
			return;
		}

		if (msg.pSender->GetName() == _T("btlz")) {
			ChangePage();
		}

		if (msg.pSender->GetName() == _T("btpause")) {
			if (0 == m_bPauseState % 2)
			{
				SetPause();
				cRcdWk.OnPause();
			}
			else
			{
				SetGoon();
				cRcdWk.OnGoon();
			}
			m_bPauseState++;
		}

		if (msg.pSender->GetName() == _T("btnCombo")) {
			CVerticalLayoutUI* cLyt = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("btlz")));
			POINT pt = { cLyt->GetPos().left + DIALOGUE_ADJ_LEFT, cLyt->GetPos().bottom + DIALOGUE_ADJ_TOP };
			ClientToScreen(m_hWnd, &pt);

			CMenuWnd* pMenu = new CMenuWnd(_T("Combo.xml"), this);
			if (pMenu == NULL) { return; }
			pMenu->Init(msg.pSender, pt);
		}
		if (msg.pSender->GetName() == _T("btpmbh")) {
			cRcdWk.OnGetScreen();
		}
		if (msg.pSender->GetName() == _T("btlzqy")) {
			CVerticalLayoutUI* cLyt = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("btlzqy")));
			POINT pt = { cLyt->GetPos().left + DIALOGUE_ADJ_LEFT, cLyt->GetPos().bottom + DIALOGUE_ADJ_TOP };
			ClientToScreen(m_hWnd, &pt);

			CMenuWnd* pMenu = new CMenuWnd(_T("area.xml"));
			if (pMenu == NULL) { return; }
			pMenu->Init(msg.pSender, pt);
		}
		if (msg.pSender->GetName() == _T("btopen")) {
			OpenRecordFile();
		}
		if (msg.pSender->GetName() == _T("btencode")) {
			CVerticalLayoutUI* cLyt = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("btencode")));
			POINT pt = { cLyt->GetPos().left + DIALOGUE_ADJ_LEFT, cLyt->GetPos().bottom + DIALOGUE_ADJ_TOP };
			ClientToScreen(m_hWnd, &pt);

			CMenuWnd* pMenu = new CMenuWnd(_T("code.xml"));
			if (pMenu == NULL) { return; }
			pMenu->Init(msg.pSender, pt);
		}
		if (msg.pSender->GetName() == _T("btvoice")) {
			CVerticalLayoutUI* cLyt = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("btvoice")));
			POINT pt = { cLyt->GetPos().left + DIALOGUE_ADJ_LEFT, cLyt->GetPos().bottom + DIALOGUE_ADJ_TOP };
			ClientToScreen(m_hWnd, &pt);

			CMenuWnd* pMenu = new CMenuWnd(_T("sound.xml"));
			if (pMenu == NULL) { return; }
			pMenu->Init(msg.pSender, pt);
		}
		if (msg.pSender->GetName() == _T("btabout")) {
			MessageBox(NULL, _T("版本 V1.1"), _T("关于"), MB_OK);
		}
	}
}

LRESULT CRecord::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

	Init();
	return 0;
}

LRESULT CRecord::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostQuitMessage(0L);

	bHandled = FALSE;
	return 0;
}

LRESULT CRecord::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (::IsIconic(*this)) bHandled = FALSE;
	return (wParam == 0) ? TRUE : FALSE;
}

LRESULT CRecord::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CRecord::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

LRESULT CRecord::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

LRESULT CRecord::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

void CRecord::OnMouseMove(UINT uMsg, HDROP hDrop, LPARAM lParam, BOOL& bHandled)
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(m_hWnd, &pt);
	if ( (pt.y < LYT_TOP) || (pt.y > LYT_BUTTON) || (pt.x < LYT_LEFT) || (pt.x >LYT_RIGHT) )
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
}

void CRecord::SetLytBkColor(LPCTSTR pLytName, int nPosX)
{
	if (pLytName == _T("lytlz"))
	{
		CHorizontalLayoutUI* cLytH = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(pLytName));
		CVerticalLayoutUI* cLytLzV = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("lytlzv")));
		CButtonUI* cBtnCom = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnCombo")));
		if ((nPosX < cLytH->GetPos().right) && (nPosX > cLytH->GetPos().left))
		{
			cLytH->SetBkColor(0xFFDDDDDD);
			cLytLzV->SetBkColor(LYT_BK_COLOR);
			cBtnCom->SetBkColor(LYT_BK_COLOR);
		}
		else
		{
			cLytH->SetBkColor(NULL);
			cLytLzV->SetBkColor(NULL);
			cBtnCom->SetBkColor(NULL);
		}
	}
	else
	{
		CVerticalLayoutUI* cLytV = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(pLytName));
		if ((nPosX < cLytV->GetPos().right) && (nPosX > cLytV->GetPos().left))
		{
			cLytV->SetBkColor(LYT_BK_COLOR);
		}
		else
		{
			cLytV->SetBkColor(NULL);
		}
	}
}

LRESULT CRecord::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_MOUSEMOVE:	   OnMouseMove(uMsg, (HDROP)wParam, lParam, bHandled); break;
	case WM_TIMER:			OnTemer();
	default:
		bHandled = FALSE;
	}
	if (bHandled) return lRes;
	if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CRecord::SetPause()
{
	CButtonUI* cBtnPause = static_cast<CButtonUI*>(m_pm.FindControl(_T("btpause")));
	CLabelUI* cLbPause = static_cast<CLabelUI*>(m_pm.FindControl(_T("lbpause")));
	cBtnPause->SetNormalImage(_T("record/goon.png"));
	cLbPause->SetText(_T("继续"));
}

void CRecord::SetGoon()
{
	CButtonUI* cBtnPause = static_cast<CButtonUI*>(m_pm.FindControl(_T("btpause")));
	CLabelUI* cLbPause = static_cast<CLabelUI*>(m_pm.FindControl(_T("lbpause")));
	cBtnPause->SetNormalImage(_T("record/player_pause.png"));
	cLbPause->SetText(_T("暂停"));
}

void CRecord::OnTemer()
{
	if (0 != m_bPauseState % 2)
		return;
	m_nRecordSecond++;
	int iSecond = m_nRecordSecond % SND_PER_MIN;
	int iMinite = ((m_nRecordSecond - iSecond) / SND_PER_MIN) % MIN_PER_HOUR;
	int iHour = (m_nRecordSecond - iSecond - iMinite * SND_PER_MIN) / (MIN_PER_HOUR * SND_PER_MIN);

	char cHour[4] = { 0 };
	char cMinite[4] = { 0 };
	char cSecond[4] = { 0 };

	if (iSecond < 10)
		sprintf(cSecond, "0%d", iSecond);
	else
		sprintf(cSecond, "%d", iSecond);

	if (iMinite < 10)
		sprintf(cMinite, "0%d", iMinite);
	else
		sprintf(cMinite, "%d", iMinite);

	if (iHour < 10)
		sprintf(cHour, "0%d", iHour);
	else
		sprintf(cHour, "%d", iHour);

	string strSnd(cSecond);
	string strMnt(cMinite);
	string strHour(cHour);

	string strTime = strHour + ":" + strMnt + ":" + strSnd;
#ifdef UNICODE
	wstring wsTime = UTF8ToUnicode(strTime);
	LPCTSTR lpTime = wsTime.c_str();
#else
	LPCTSTR lpTime = strTime.c_str();
#endif

	CLabelUI* cLbl = static_cast<CLabelUI*>(m_pm.FindControl(_T("LbTime")));
	cLbl->SetText(lpTime);
}

void CRecord::ChangePage()
{
	CRecordWork cRcdWk;
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

		cRcdWk.m_bMcf = m_bMcf;
		cRcdWk.m_bScreenRecord = m_bScreenRecord;
		cRcdWk.m_bSoundRecord = m_bSoundRecord;
		cRcdWk.m_bSysSound = m_bSysSound;
		cRcdWk.m_iCode = m_iCode;
		cRcdWk.m_rArea = m_rArea;
		cRcdWk.OnRecord();
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
		cRcdWk.OnStop();
		m_bPauseState = false;
	}
	i++;
}

void CRecord::ScreenToSound()
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

void CRecord::SoundToScreen()
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

void CRecord::SetSaveDir()
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
}

void CRecord::OpenRecordFile()
{
	TCHAR szBuffer[MAX_PATH] = { 0 };
	OPENFILENAME ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = _T("所有文件(*.*)\0*.*\0所有文件(*.*)\0*.*\0");//要选择的文件后缀   
	ofn.lpstrInitialDir = _T("D:\\Program Files");//默认的文件路径   
	ofn.lpstrFile = szBuffer;//存放文件的缓冲区   
	ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;//标志如果是多选要加上OFN_ALLOWMULTISELECT  
	BOOL bSel = GetOpenFileName(&ofn);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int nCmdShow)
{

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));

	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;

	CRecord* pFrame = new CRecord();
	if (pFrame == NULL) return 0;
	pFrame->Create(NULL, NULL, UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE | WS_EX_ACCEPTFILES);
	pFrame->SetIcon(IDI_ICON1);
	pFrame->CenterWindow();
	pFrame->ShowWindow(true);
	CPaintManagerUI::MessageLoop();

	::CoUninitialize();
	return 0;
}
