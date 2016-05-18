#pragma once
#include <map>
#include <wtypes.h>
#include "resource.h"
#include <vector>
#include "atlimage.h"
#include <Shellapi.h>

#include "MenuWnd.h"

class CNewVerticalLayoutUI :public CVerticalLayoutUI
{
public:
	CNewVerticalLayoutUI(){};
	~CNewVerticalLayoutUI(){};
private:
	void CNewVerticalLayoutUI::DoEvent(TEventUI& event)
	{
		if (event.Type == UIEVENT_BUTTONDOWN)
		{
			m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
		}
		if (m_iSepHeight != 0) {
			if (event.Type == UIEVENT_BUTTONDOWN && IsEnabled())
			{
				RECT rcSeparator = GetThumbRect(false);
				if (::PtInRect(&rcSeparator, event.ptMouse)) {
					m_uButtonState |= UISTATE_CAPTURED;
					m_ptLastMouse = event.ptMouse;
					m_rcNewPos = m_rcItem;
					if (!m_bImmMode && m_pManager) m_pManager->AddPostPaint(this);

					return;
				}
			}
			if (event.Type == UIEVENT_BUTTONUP)
			{
				if ((m_uButtonState & UISTATE_CAPTURED) != 0) {
					m_uButtonState &= ~UISTATE_CAPTURED;
					m_rcItem = m_rcNewPos;
					if (!m_bImmMode && m_pManager) m_pManager->RemovePostPaint(this);
					NeedParentUpdate();
					return;
				}
			}
			if (event.Type == UIEVENT_MOUSEMOVE)
			{
				if ((m_uButtonState & UISTATE_CAPTURED) != 0) {
					LONG cy = event.ptMouse.y - m_ptLastMouse.y;
					m_ptLastMouse = event.ptMouse;
					RECT rc = m_rcNewPos;
					if (m_iSepHeight >= 0) {
						if (cy > 0 && event.ptMouse.y < m_rcNewPos.bottom + m_iSepHeight) return;
						if (cy < 0 && event.ptMouse.y > m_rcNewPos.bottom) return;
						rc.bottom += cy;
						if (rc.bottom - rc.top <= GetMinHeight()) {
							if (m_rcNewPos.bottom - m_rcNewPos.top <= GetMinHeight()) return;
							rc.bottom = rc.top + GetMinHeight();
						}
						if (rc.bottom - rc.top >= GetMaxHeight()) {
							if (m_rcNewPos.bottom - m_rcNewPos.top >= GetMaxHeight()) return;
							rc.bottom = rc.top + GetMaxHeight();
						}
					}
					else {
						if (cy > 0 && event.ptMouse.y < m_rcNewPos.top) return;
						if (cy < 0 && event.ptMouse.y > m_rcNewPos.top + m_iSepHeight) return;
						rc.top += cy;
						if (rc.bottom - rc.top <= GetMinHeight()) {
							if (m_rcNewPos.bottom - m_rcNewPos.top <= GetMinHeight()) return;
							rc.top = rc.bottom - GetMinHeight();
						}
						if (rc.bottom - rc.top >= GetMaxHeight()) {
							if (m_rcNewPos.bottom - m_rcNewPos.top >= GetMaxHeight()) return;
							rc.top = rc.bottom - GetMaxHeight();
						}
					}

					CDuiRect rcInvalidate = GetThumbRect(true);
					m_rcNewPos = rc;
					m_cxyFixed.cy = m_rcNewPos.bottom - m_rcNewPos.top;

					if (m_bImmMode) {
						m_rcItem = m_rcNewPos;
						NeedParentUpdate();
					}
					else {
						rcInvalidate.Join(GetThumbRect(true));
						rcInvalidate.Join(GetThumbRect(false));
						if (m_pManager) m_pManager->Invalidate(rcInvalidate);
					}
					return;
				}
			}
			if (event.Type == UIEVENT_SETCURSOR)
			{
				RECT rcSeparator = GetThumbRect(false);
				if (IsEnabled() && ::PtInRect(&rcSeparator, event.ptMouse)) {
					::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
					return;
				}
			}
		}
		CContainerUI::DoEvent(event);
	}
};


typedef struct
{
	CNewVerticalLayoutUI* Layout;
	char FilePath[100];
}LayOut_Info;

class Launcher : public CWindowWnd, public INotifyUI
{
public:
	Launcher();
	~Launcher();

protected:
	LPCTSTR GetWindowClassName() const { return _T("UILauncher"); };
	UINT GetClassStyle() const { return UI_CLASSSTYLE_DIALOG; };
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };
	void Init();
	void Notify(TNotifyUI& msg);

	void c2w(wchar_t *pwstr, size_t len, const char *str);
	void w2c(char *pcstr, const wchar_t *pwstr, int len);

	void SaveLytToJsonFile();
	void PopMenu(TNotifyUI& msg);
	void DeleteLyt();
	void OpenExeFile(int xPos, int yPos);

	HICON QueryFileIcon(LPCTSTR lpszFilePath);
	void MapInit();
	void GetIcon(const char* strPath);

	void AddToMap(LPCTSTR LayoutName);
	void LayMove(CNewVerticalLayoutUI* cLyt, int nMove);
	void Vacated_position(int iPos);
	void AddLayout(int nPosX, int nPosY, LPCTSTR pFileName, LPCTSTR strName);

	void InitLayOut(CNewVerticalLayoutUI* cLyt, LPCTSTR pFileName, LPCTSTR strName);
	void Push_LayOut(int xPos, int yPos, CNewVerticalLayoutUI* cLyt, LPCTSTR strName);
	void ShowLayOut();

	HBITMAP IconToBitmap(HICON hIcon, SIZE* pTargetSize = NULL);
	BOOL SaveBmp(HBITMAP hBitmap, LPCSTR FileName);

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDropFiles(UINT uMsg, HDROP hDrop, LPARAM lParam, BOOL& bHandled);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	CPaintManagerUI m_pm;
	POINT m_MenuPt;
	POINT m_xyPos;
	int m_Nbmp = 1;
	vector<LayOut_Info> m_AllLyt;
	HWND hListBoxWnd;

};

