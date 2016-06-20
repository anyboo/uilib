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
	if (bTag)
	{
		CComboUI* Comb = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("CB_month")));
		m_PrevMonth = Comb->GetCurSel() + 1;
		OnPrepare();
		bTag = false;
	}
	if (msg.sType == DUI_MSGTYPE_ITEMSELECT && msg.pSender->GetName() == _T("CB_month"))
	{
		CComboUI* Comb = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("CB_month")));
		int newmonth = Comb->GetCurSel() + 1;
		if (newmonth > m_PrevMonth){
			m_sysTime.wMonth = m_sysTime.wMonth + newmonth - m_PrevMonth;
			DrawCalendar(m_sysTime);
		}
		else if(newmonth < m_PrevMonth){
			m_sysTime.wMonth = m_sysTime.wMonth + newmonth - m_PrevMonth;
			DrawCalendar(m_sysTime);
		}
		m_PrevMonth = newmonth;		
	}
	if (msg.sType == DUI_MSGTYPE_KILLFOCUS && msg.pSender->GetName() == _T("InputYear"))
	{
		
		int a = 0;
	}
	WindowImplBase::Notify(msg);
}

void CalendarUI::OnClose(TNotifyUI& msg)
{
	Close();
}

void CalendarUI::OnPrepare()
{
	::GetLocalTime(&m_sysTime);
	DrawCalendar(m_sysTime);
}

//void Notify(TNotifyUI& msg)
//{
//	if (msg.sType == _T("windowinit")) OnPrepare();
//	else if (msg.sType == _T("return"))
//	{
//		m_sysTime.wYear = SetTxtYear(0);
//		m_sysTime.wMonth = GetCmbMonth();
//		DrawCalendar(m_sysTime);
//	}
//	else if (msg.sType == _T("click"))
//	{
//		CControlUI* pOne = static_cast<CControlUI*>(m_pm.FindControl(msg.ptMouse));
//		if (_tcsicmp(pOne->GetClass(), _T("ButtonUI")) == 0)
//		{
//			//上一月
//			if (_tcsicmp(pOne->GetName(), _T("BTN_UP_MONTH")) == 0)
//			{
//				m_sysTime.wMonth = m_sysTime.wMonth - 1 == 0 ? 12 : m_sysTime.wMonth - 1;
//				DrawCalendar(m_sysTime);
//			}
//			//上一年
//			else if (_tcsicmp(pOne->GetName(), _T("BTN_UP_YEAR")) == 0)
//			{
//				m_sysTime.wYear = SetTxtYear(-1);
//				m_sysTime.wMonth = GetCmbMonth();
//				DrawCalendar(m_sysTime);
//			}
//			//下一年
//			else if (_tcsicmp(pOne->GetName(), _T("BTN_DOWN_YEAR")) == 0)
//			{
//				m_sysTime.wYear = SetTxtYear(1);
//				m_sysTime.wMonth = GetCmbMonth();
//				DrawCalendar(m_sysTime);
//			}
//		}
//	}
//	else if (msg.sType == _T("itemselect")) {
//		if (_tcsicmp(msg.pSender->GetName(), _T("CMB_MONTH")) == 0)
//		{
//			m_sysTime.wMonth = GetCmbMonth();
//			DrawCalendar(m_sysTime);
//		}
//	}
//}

STDSTRING CalendarUI::intToString(int num)
{
	char Tmp[10] = { 0 };
	_itoa(num, Tmp, 10);
	STDSTRING result(Tmp);
	return result;
}

//绘制日历
void CalendarUI::DrawCalendar(SYSTEMTIME m_sysTime)
{
	int iDay = 0;
	STDSTRING cDay, cMonth, cYear;
	STDSTRING itemName, BtnName;
	//int iStartDay = m_sysTime.wDayOfWeek;
	m_sysTime.wDay = 1;
	int iStartDay = GetDayOfWeek(m_sysTime);
	//上月天数
	int iLastMonthStartDays = 31 - iStartDay;
	if (m_sysTime.wMonth > 1)
		iLastMonthStartDays = GetMonthDays(m_sysTime.wYear, m_sysTime.wMonth - 1) - iStartDay;
	//本月天数
	int iMonthDays = GetMonthDays(m_sysTime.wYear, m_sysTime.wMonth);
	//下月天数
	int iNextMonthDays = 0;
	
	for (int i = 0; i < 42; i++)
	{
		BtnName = "Button" + intToString(i);
		CButtonUI* btn = static_cast<CButtonUI*>(m_PaintManager.FindControl(BtnName.c_str()));
		//上月
		if (i<iStartDay)
		{
			iLastMonthStartDays++;
			cDay = intToString(iLastMonthStartDays);
			btn->SetText(cDay.c_str());
			btn->SetBkColor(0x64263232);
		}
		else if (i>iStartDay - 1 && iDay < iMonthDays)
		{
			iDay++;
			cDay = intToString(iDay);
			btn->SetText(cDay.c_str());
			btn->SetBkColor(0xc3324534);
		}
		else
		{
			iNextMonthDays++;
			cDay = intToString(iNextMonthDays);
			btn->SetText(cDay.c_str());
			btn->SetBkColor(0x64263232);
		}
	}

	cMonth = intToString(m_sysTime.wMonth);
	itemName = "Item" + cMonth;
	CListLabelElementUI* itemLabe = static_cast<CListLabelElementUI*>(m_PaintManager.FindControl(itemName.c_str()));
	itemLabe->Select(true);

	cYear = intToString(m_sysTime.wYear);
	CEditUI* edit_year = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("InputYear")));
	edit_year->SetText(cYear.c_str());
}


int CalendarUI::GetMonthDays(int iY, int iM)
{
	int iTotalDay = 31;
	if (iM == 2)
	{
		//闰年可以被4或者400整除 但是不能被100整除
		if (iY % 4 == 0 && iY % 100 != 0 || iY % 400 == 0)
			iTotalDay = 29;
		else
			iTotalDay = 28;
	}
	else if (iM == 4 || iM == 6 || iM == 9 || iM == 11)
		iTotalDay = 30;
	return iTotalDay;
}

int CalendarUI::GetDayOfWeek(SYSTEMTIME m_sysTime)
{
	m_ctime.SetDate(m_sysTime.wYear, m_sysTime.wMonth, m_sysTime.wDay);
	return m_ctime.GetDayOfWeek() - 1;
}

//int GetCmbMonth()
//{
//	CComboUI* pCmb = static_cast<CComboUI*>(m_pm.FindControl(_T("CMB_MONTH")));
//	CDuiString cCurMonth = pCmb->GetText();
//	return atoi(cCurMonth.Left(cCurMonth.GetLength() - 1));
//}
//
//int SetTxtYear(int iY)
//{
//	CEditUI* pTxt = static_cast<CEditUI*>(m_pm.FindControl(_T("TXT_YEAR")));
//	CDuiString cCurYear = pTxt->GetText();
//	int iCurYear = 0;
//	iCurYear = atoi(cCurYear) + iY;
//	cCurYear.Format("%d", iCurYear);
//	pTxt->SetText(cCurYear);
//	return iCurYear;
//}
