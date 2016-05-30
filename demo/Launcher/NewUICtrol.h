#pragma once

#include "StdAfx.h"


class CNewVerticalLayoutUI :public CVerticalLayoutUI
{
public:
	CNewVerticalLayoutUI(){};
	~CNewVerticalLayoutUI(){};
private:
	void CNewVerticalLayoutUI::DoEvent(TEventUI& event)
	{
		if (event.Type == UIEVENT_BUTTONUP)
		{
			m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
		}
		if (event.Type == UIEVENT_MOUSEMOVE)
		{
			m_pManager->SendNotify(this, DUI_MSGTYPE_TABSELECT);
		}
		if (event.Type == UIEVENT_MOUSELEAVE){
			m_pManager->SendNotify(this, DUI_MSGTYPE_SELECTCHANGED);
		}
		CContainerUI::DoEvent(event);
	}
};

class CNewButtonUI : public CButtonUI 
{
public:
	CNewButtonUI(){};
	~CNewButtonUI(){};

public:
	void CNewButtonUI::DoEvent(TEventUI& event)
	{
		if (event.Type == UIEVENT_MOUSEMOVE)
		{
			m_pManager->SendNotify(this, DUI_MSGTYPE_TABSELECT);
			if ((m_uButtonState & UISTATE_CAPTURED) != 0) {
				if (::PtInRect(&m_rcItem, event.ptMouse)) m_uButtonState |= UISTATE_PUSHED;
				else m_uButtonState &= ~UISTATE_PUSHED;
				Invalidate();
			}
			return;
		}
		if (event.Type == UIEVENT_SETCURSOR)
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
			return;
		}
		CLabelUI::DoEvent(event);
	}
};

typedef struct
{
	CNewVerticalLayoutUI* Layout;
	STDSTRING FilePath;
	STDSTRING Display;
}LayOut_Info;


