#include "stdafx.h"
#include "MainWnd.h"


CMainWnd::CMainWnd()
:tmp(_T("A") )
{
	
}


CMainWnd::~CMainWnd()
{
	
}


DUI_BEGIN_MESSAGE_MAP(CMainWnd, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_CLOSE, OnClose)
DUI_ON_CLICK_CTRNAME(BT_MINWIND, OnMin)
DUI_END_MESSAGE_MAP()

LPCTSTR CMainWnd::GetWindowClassName() const
{
	return _T("launcher");
}

CDuiString CMainWnd::GetSkinFolder()
{
	return _T("skin\\");
}

CDuiString CMainWnd::GetSkinFile()
{
	return _T("launcher.xml");
}

void CMainWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CMainWnd::OnClose(TNotifyUI& msg)
{
	m_config.SaveToConfig(m_AllLyt);
	::PostQuitMessage(0L);
}

void CMainWnd::OnMin(TNotifyUI& msg)
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainWnd::Notify(TNotifyUI& msg)
{
	if (begin == 1){
		m_config.LoadConfig(m_AllLyt, m_PaintManager);
		begin++;
	}
	if (msg.sType == _T("menu")){
		menuPos = { msg.ptMouse.x, msg.ptMouse.y };
		m_MyHandle.PopMenu(msg, m_hWnd);
	}
	else if (msg.sType == _T("menu_Delete")){
		m_MyHandle.DeleteLyt(m_PaintManager, m_AllLyt);
	}
	else if (msg.sType == _T("menu_Open")){
		m_MyHandle.OpenExeFile(menuPos.x, menuPos.y, m_AllLyt);
	}
	else if (msg.sType == _T("tabselect")){
		m_MyHandle.OnMouseMove(msg.ptMouse.x, msg.ptMouse.y, m_AllLyt);
	}
	else if (msg.sType == _T("selectchanged")){
		m_MyHandle.OnMouseLeave(m_AllLyt);
	}
	else if (msg.sType == _T("click")){
		m_MyHandle.OpenExeFile(msg.ptMouse.x, msg.ptMouse.y, m_AllLyt);
	}
	return WindowImplBase::NotifyPump(msg);
}

LRESULT CMainWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch (uMsg) {
	case WM_DROPFILES:	    lRes = OnDropFiles(uMsg, HDROP(wParam), lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if (bHandled) return lRes;
	if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
	return WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMainWnd::OnDropFiles(UINT uMsg, HDROP hDrop, LPARAM lParam, BOOL& bHandled)
{
	POINT* ptDropPos = new POINT;
	DragQueryPoint(hDrop, ptDropPos);	
	int iDropPos = ptDropPos->x;
	int yDropPos = ptDropPos->y;
	delete(ptDropPos);

	HICON hIcon = NULL;
	STDSTRING BmpFileName, strFilePath;
#ifdef _UNICODE
	wchar_t strFileName[MAX_PATH] = { 0 };
#else
	char strFileName[MAX_PATH] = { 0 };
#endif
	WORD wNumFilesDropped = DragQueryFile(hDrop, -1, NULL, 0);
	for (unsigned short i = 0; i < wNumFilesDropped; i++)
	{	
		tmp[0] += 1;
		BmpFileName = tmp + _T("tmp.bmp");		
		DragQueryFile(hDrop, i, strFileName, MAX_PATH);
		strFilePath = strFileName;	
		hIcon = m_MyHandle.QueryFileIcon(strFilePath);
		HBITMAP IconHbmp = m_MyHandle.IconToBitmap(hIcon);
		m_MyHandle.SaveBmp(IconHbmp, BmpFileName);
		m_MyHandle.AddLayout(iDropPos, yDropPos, BmpFileName, strFilePath, m_PaintManager, m_AllLyt);
	}
	return 0;
}