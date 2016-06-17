#include "stdafx.h"
#include "LogUI.h"


CLogUI::CLogUI()
{
}

CLogUI::~CLogUI()
{
}

DUI_BEGIN_MESSAGE_MAP(CLogUI, WindowImplBase)
DUI_END_MESSAGE_MAP()

LPCTSTR CLogUI::GetWindowClassName() const
{
	return _T("CLogUI");
}

CDuiString CLogUI::GetSkinFolder()
{
	return _T("..//x86");
}

CDuiString CLogUI::GetSkinFile()
{
	return _T("LogUI.xml");
}

void CLogUI::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CLogUI::Notify(TNotifyUI& msg)
{
	WindowImplBase::Notify(msg);
}