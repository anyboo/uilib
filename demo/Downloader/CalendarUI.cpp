#include "stdafx.h"
#include "CalendarUI.h"


CalendarUI::CalendarUI()
{
}


CalendarUI::~CalendarUI()
{
}

DUI_BEGIN_MESSAGE_MAP(CalendarUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CANCEL, OnClose)
DUI_END_MESSAGE_MAP()

LPCTSTR CalendarUI::GetWindowClassName() const
{
	return _T("CLogUI");
}

CDuiString CalendarUI::GetSkinFolder()
{
	return _T("..//x86");
}

CDuiString CalendarUI::GetSkinFile()
{
	return _T("CalendarUI.xml");
}

void CalendarUI::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CalendarUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		Close();
	}
	WindowImplBase::Notify(msg);
}

void CalendarUI::OnClose(TNotifyUI& msg)
{
	Close();
}