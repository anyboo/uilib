#pragma once
#include <DuiLib/UIlib.h>

class CShadowWnd : 
	public CWindowWnd
{
private:
    bool m_bNeedUpdate;
	TDrawInfo m_di;
    CPaintManagerUI m_pm;

public:
	CShadowWnd();
	LPCTSTR GetWindowClassName() const;
    UINT GetClassStyle() const;
    void OnFinalMessage(HWND /*hWnd*/);
	void RePaint();
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

};

class CMenuWnd : 
	public CWindowWnd, public INotifyUI
{
public:
	CMenuWnd();

	void Init(CControlUI* pOwner, POINT pt);

	void AdjustPostion();

    LPCTSTR GetWindowClassName() const;

    void OnFinalMessage(HWND /*hWnd*/);

	void Notify(TNotifyUI& msg);

	HWND Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int cx = CW_USEDEFAULT, int cy = CW_USEDEFAULT, HMENU hMenu = NULL);

	void ShowWindow(bool bShow, bool bTakeFocus);

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void ChooseCode(LPCTSTR pLbName);

	void ChooseLzPage(LPCTSTR pLbName);

	void SetChooseCode(LPCTSTR pDesLbName, LPCTSTR pLbName);

	void SetChooseLzPage(LPCTSTR pDesLbName, LPCTSTR pLbName);

	void OnPrepare();

public:
    CPaintManagerUI m_pm;
    CControlUI* m_pOwner;
    POINT m_ptPos;
    CShadowWnd* m_pShadowWnd;
	LPCTSTR m_pFileName;
	//CMainWnd* m_Record;
};
