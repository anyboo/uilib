// App.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <wtypes.h>
#include <shellapi.h>
#include <map>
#include "resource.h"
#include <vector>
#include "MenuWnd.h"
using namespace std;

#define DIALOGUE_ADJ_LEFT -20
#define DIALOGUE_ADJ_TOP 12

#define LYT_TOP 35
#define LYT_BUTTON 100
#define LYT_LEFT 5
#define LYT_RIGHT 512

#define LYT_BK_COLOR 0xFFFF9999

#define PAGE_RECORD 1
#define PAGE_RECORDING 2

class CRecord : public CWindowWnd, public INotifyUI
{
public:
	CRecord() { };
	LPCTSTR GetWindowClassName() const { return _T("UIRecord"); };
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };

	void Init()
	{
	}

	void Notify(TNotifyUI& msg)
	{
		if (msg.sType == _T("click")) {
			if (msg.pSender->GetName() == _T("closebtn")) {
				PostQuitMessage(0);
				return;
			}
			if (msg.pSender->GetName() == _T("minbtn")) {
				SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
				return;
			}

			if (msg.pSender->GetName() == _T("btlz")) {
				ChangePage();
			}
			if (msg.pSender->GetName() == _T("btnCombo")) {
				CVerticalLayoutUI* cLyt = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(_T("btlz")));
				POINT pt = { cLyt->GetPos().left + DIALOGUE_ADJ_LEFT, cLyt->GetPos().bottom + DIALOGUE_ADJ_TOP };
				ClientToScreen(m_hWnd, &pt);

				CMenuWnd* pMenu = new CMenuWnd(_T("Combo.xml"));
				if (pMenu == NULL) { return; }
				pMenu->Init(msg.pSender, pt);
			}
			if (msg.pSender->GetName() == _T("btpmbh")) {
				MessageBox(NULL, _T("Record Capture"), _T("message"), MB_OK);
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
				MessageBox(NULL, _T("open"), _T("message"), MB_OK);
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
				MessageBox(NULL, _T("°æ±¾ V1.0"), _T("¹ØÓÚ"), MB_OK);
			}
		}
	}

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		::PostQuitMessage(0L);

		bHandled = FALSE;
		return 0;
	}

	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		if (::IsIconic(*this)) bHandled = FALSE;
		return (wParam == 0) ? TRUE : FALSE;
	}

	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
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

	void OnMouseMove(UINT uMsg, HDROP hDrop, LPARAM lParam, BOOL& bHandled)
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

	void SetLytBkColor(LPCTSTR pLytName, int nPosX)
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

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		default:
			bHandled = FALSE;
		}
		if (bHandled) return lRes;
		if (m_pm.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

	void ChangePage()
	{
		static int i = 0;
		if (0 == i)
			m_pPage1 = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("page1")));
		CHorizontalLayoutUI* cSelectPage = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(_T("selectPage")));
		if (i % 2 == 0)
		{
			cSelectPage->Remove(m_pPage1, true);
			cSelectPage->Add(m_pPage2);
			m_nPageState = PAGE_RECORDING;
		}
		else
		{
			cSelectPage->Remove(m_pPage2, true);
			cSelectPage->Add(m_pPage1);
			m_nPageState = PAGE_RECORD;
		}
		i++;
	}

	void ScreenToSound()
	{
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

		i++;
	}

	void SoundToScreen()
	{
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

	}

public:
	CPaintManagerUI m_pm;
	CHorizontalLayoutUI* m_pSelectPage;
	CHorizontalLayoutUI* m_pPage1;
	CControlUI* m_pPage2;
	CVerticalLayoutUI* m_JustScreenLyt[6];
	int m_nPageState = PAGE_RECORD;
};

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
