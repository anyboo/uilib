#pragma once
#include <wtypes.h>
#include <shellapi.h>
#include "MenuWnd.h"
#include "record.h"

class CSetPageDialog :
	public CWindowWnd, public INotifyUI, public IMessageFilterUI
{
public:
	CSetPageDialog(){};
	CSetPageDialog(CRecord* cRcd);
	~CSetPageDialog();
	LPCTSTR GetWindowClassName() const;
	UINT GetClassStyle() const;
	void OnFinalMessage(HWND /*hWnd*/);

	void Init();

	void Notify(TNotifyUI& msg);

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	void OnPrepare();

public:
	CPaintManagerUI m_pm;
	CRecord* m_cRcd;
};

