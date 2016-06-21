#pragma once
#include <DuiLib/UIlib.h>
#include "DownLoadWnd.h"

#include <ATLComTime.h>

#define BT_CANCEL			(_T("closebtn1"))


class CalendarUI :
	public WindowImplBase
{
public:
	CalendarUI();
	~CalendarUI();
	virtual void OnFinalMessage(HWND /*hWnd*/);
	virtual void Notify(TNotifyUI& msg);
	DUI_DECLARE_MESSAGE_MAP();
	void OnClose(TNotifyUI& msg);

	STDSTRING intToString(int num);

	void OnPrepare();
	void DrawCalendar(SYSTEMTIME m_sysTime);
	int GetDayOfWeek(SYSTEMTIME m_sysTime);
	int GetMonthDays(int iY, int iM);

private:
	BOOL			m_bTag;
	SYSTEMTIME		 m_sysTime;
	COleDateTime	m_ctime;
	int				 m_PrevMonth;
protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
};
