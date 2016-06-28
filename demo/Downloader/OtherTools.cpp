#include "stdafx.h"
#include "OtherTools.h"


COtherTools::COtherTools()
{
}


COtherTools::~COtherTools()
{
}

DUI_BEGIN_MESSAGE_MAP(COtherTools, WindowImplBase)

DUI_END_MESSAGE_MAP()

LPCTSTR COtherTools::GetWindowClassName() const
{
	return _T("CLogUI");
}

CDuiString COtherTools::GetSkinFolder()
{
	return _T("..//x86");
}

CDuiString COtherTools::GetSkinFile()
{
	return _T("OtherTools.xml");
}

void COtherTools::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void COtherTools::Notify(TNotifyUI& msg)
{

	WindowImplBase::Notify(msg);
}
