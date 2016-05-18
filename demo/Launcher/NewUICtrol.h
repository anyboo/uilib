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
		if (m_iSepHeight == 0)
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
		}
		if (m_iSepHeight != 0) {
			if (event.Type == UIEVENT_BUTTONDOWN && IsEnabled())
			{
				m_pManager->SendNotify(this, DUI_MSGTYPE_CLICK);
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
				m_pManager->SendNotify(this, DUI_MSGTYPE_TABSELECT);
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


class CNewButtonUI : public CButtonUI 
{
public:
	CNewButtonUI(){};
	~CNewButtonUI(){};

public:
	void CNewButtonUI::DoEvent(TEventUI& event)
	{
		if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND) {
			if (m_pParent != NULL) m_pParent->DoEvent(event);
			else CLabelUI::DoEvent(event);
			return;
		}

		if (event.Type == UIEVENT_SETFOCUS)
		{
			Invalidate();
		}
		if (event.Type == UIEVENT_KILLFOCUS)
		{
			Invalidate();
		}
		if (event.Type == UIEVENT_KEYDOWN)
		{
			if (IsKeyboardEnabled() && IsEnabled()) {
				if (event.chKey == VK_SPACE || event.chKey == VK_RETURN) {
					Activate();
					return;
				}
			}
		}
		if (event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK)
		{
			if (::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled()) {
				m_uButtonState |= UISTATE_PUSHED | UISTATE_CAPTURED;
				Invalidate();
			}
			return;
		}
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
		if (event.Type == UIEVENT_BUTTONUP)
		{
			if ((m_uButtonState & UISTATE_CAPTURED) != 0) {
				if (::PtInRect(&m_rcItem, event.ptMouse) && IsEnabled()) Activate();
				m_uButtonState &= ~(UISTATE_PUSHED | UISTATE_CAPTURED);
				Invalidate();
			}
			return;
		}
		if (event.Type == UIEVENT_CONTEXTMENU)
		{
			if (IsContextMenuUsed() && IsEnabled()) {
				m_pManager->SendNotify(this, DUI_MSGTYPE_MENU, event.wParam, event.lParam);
			}
			return;
		}
		if (event.Type == UIEVENT_MOUSEENTER)
		{
			if (::PtInRect(&m_rcItem, event.ptMouse)) {
				if (IsEnabled()) {
					if ((m_uButtonState & UISTATE_HOT) == 0) {
						m_uButtonState |= UISTATE_HOT;
						Invalidate();
					}
				}
			}
			if (GetFadeAlphaDelta() > 0) {
				m_pManager->SetTimer(this, FADE_TIMERID, FADE_ELLAPSE);
			}
		}
		if (event.Type == UIEVENT_MOUSELEAVE)
		{
			if (!::PtInRect(&m_rcItem, event.ptMouse)) {
				if (IsEnabled()) {
					if ((m_uButtonState & UISTATE_HOT) != 0) {
						m_uButtonState &= ~UISTATE_HOT;
						Invalidate();
					}
				}
				if (m_pManager) m_pManager->RemoveMouseLeaveNeeded(this);
				if (GetFadeAlphaDelta() > 0) {
					m_pManager->SetTimer(this, FADE_TIMERID, FADE_ELLAPSE);
				}
			}
			else {
				if (m_pManager) m_pManager->AddMouseLeaveNeeded(this);
				return;
			}
		}
		if (event.Type == UIEVENT_SETCURSOR)
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND)));
			return;
		}
		if (event.Type == UIEVENT_TIMER  && event.wParam == FADE_TIMERID)
		{
			if ((m_uButtonState & UISTATE_HOT) != 0) {
				if (m_uFadeAlpha > m_uFadeAlphaDelta) m_uFadeAlpha -= m_uFadeAlphaDelta;
				else {
					m_uFadeAlpha = 0;
					m_pManager->KillTimer(this, FADE_TIMERID);
				}
			}
			else {
				if (m_uFadeAlpha < 255 - m_uFadeAlphaDelta) m_uFadeAlpha += m_uFadeAlphaDelta;
				else {
					m_uFadeAlpha = 255;
					m_pManager->KillTimer(this, FADE_TIMERID);
				}
			}
			Invalidate();
			return;
		}
		CLabelUI::DoEvent(event);
	}
};


