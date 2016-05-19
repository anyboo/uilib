#pragma once
#include <DuiLib/UIlib.h>

class CSettingWnd :
	public CWindowWnd, public INotifyUI
{
public:
	CSettingWnd();
	~CSettingWnd();

	virtual void Notify(TNotifyUI& msg);

protected:
	LPCTSTR GetWindowClassName() const;
	//UINT GetClassStyle() const;

	virtual void OnFinalMessage(HWND /*hWnd*/);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	void Init();
	void OnPrepare();

private:
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);
	//void OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	CPaintManagerUI* pm;
};

