#include "stdafx.h"
#include "VideoLoginUI.h"


VideoLoginUI::VideoLoginUI()
{
}


VideoLoginUI::~VideoLoginUI()
{
}

DUI_BEGIN_MESSAGE_MAP(VideoLoginUI, WindowImplBase)
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
	WindowImplBase::Notify(msg);
}