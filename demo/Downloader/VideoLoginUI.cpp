#include "stdafx.h"
#include "VideoLoginUI.h"
#include "VideoVendorUI.h"

VideoLoginUI::VideoLoginUI()
{
}


VideoLoginUI::~VideoLoginUI()
{
}

DUI_BEGIN_MESSAGE_MAP(VideoLoginUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_VideoVendor, OnOpenVideoVendorWnd)
DUI_END_MESSAGE_MAP()

LPCTSTR VideoLoginUI::GetWindowClassName() const
{
	return _T("CLogUI");
}

CDuiString VideoLoginUI::GetSkinFolder()
{
	return _T("..//x86");
}

CDuiString VideoLoginUI::GetSkinFile()
{
	return _T("VideoLoginUI.xml");
}

void VideoLoginUI::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void VideoLoginUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK)
	{
		if (msg.pSender->GetName() == _T("IP_match"))
		{
			POINT pt = { msg.ptMouse.x, msg.ptMouse.y };
			int x = pt.x;
		}		
	}
	WindowImplBase::Notify(msg);
}

void VideoLoginUI::OnOpenVideoVendorWnd(TNotifyUI& msg)
{
	CVideoVendorUI* pDlg = new CVideoVendorUI();
	assert(pDlg);
	pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_EX_DIALOG, 0L, 0, 0, 0, 0);
	pDlg->CenterWindow();
	pDlg->ShowModal();
}