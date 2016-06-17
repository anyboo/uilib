#pragma once
#include <DuiLib/UIlib.h>
#include "DownLoadWnd.h"

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


protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
};
