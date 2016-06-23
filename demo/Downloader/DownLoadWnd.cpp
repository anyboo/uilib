#include "stdafx.h"
#include "DownLoadWnd.h"
#include "CalendarUI.h"
#include "VideoLoginUI.h"

DownLoadWnd::DownLoadWnd()
{
}


DownLoadWnd::~DownLoadWnd()
{
}


DUI_BEGIN_MESSAGE_MAP(DownLoadWnd, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_OnVideoLoginUI, OnVideoLoginWnd)
DUI_END_MESSAGE_MAP()

LPCTSTR DownLoadWnd::GetWindowClassName() const
{
	return _T("DownLoadWnd");
}

CDuiString DownLoadWnd::GetSkinFolder()
{
	return _T("..//x86");
}

CDuiString DownLoadWnd::GetSkinFile()
{
	return _T("Download.xml");
}

void DownLoadWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void DownLoadWnd::OnSelectTimeType()
{
	CSliderUI* Slider = static_cast<CSliderUI*>(m_PaintManager.FindControl(_T("Select_time")));
	if (Slider->GetValue() > 50)
	{
		Slider->SetValue(100);
	}
	else
	{
		Slider->SetValue(0);
	}
}

void DownLoadWnd::OnSelectCalendar()
{
	CalendarUI* pDlg = new CalendarUI();
	assert(pDlg);
	pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_EX_DIALOG, 0L, 0, 0, 350, 380);
	pDlg->CenterWindow();
	pDlg->ShowModal();
}

void DownLoadWnd::OnVideoLoginWnd(TNotifyUI& msg)
{
	VideoLoginUI* pDlg = new VideoLoginUI();
	assert(pDlg);
	pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_EX_DIALOG, 0L, 0, 0, 1024, 768);
	pDlg->CenterWindow();
	pDlg->ShowModal();
}

void DownLoadWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_VALUECHANGED && msg.pSender->GetName() == _T("Select_time")){
		OnSelectTimeType();
	}
	if (msg.sType == DUI_MSGTYPE_CLICK){
		if (msg.pSender->GetName() == BT_Calendar1 || msg.pSender->GetName() == BT_Calendar2){
			OnSelectCalendar();
		}
		if (msg.pSender->GetName() == _T("Search")){
			SearchFiles();
			ShowFileList();
		}
		if (msg.pSender->GetName() == _T("All"))
		{
			COptionUI* option = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("option1")));
			option->Selected(true);
		}
	}
	WindowImplBase::Notify(msg);
}


void DownLoadWnd::SearchFiles()
{
	m_fileInfo.clear();
	//search files
}

void DownLoadWnd::ShowFileList()
{
	
	CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("domainlist")));
	CListContainerElementUI* Sublist = new CListContainerElementUI;
	CHorizontalLayoutUI* hLyt = new CHorizontalLayoutUI;
	COptionUI* option = new COptionUI;
	CLabelUI* Lab_Name = new CLabelUI;
	int num = pList->GetCount();
	pList->Add(Sublist);
	
	Sublist->SetFixedHeight(30);
	Sublist->Add(hLyt);
	hLyt->Add(option);
	hLyt->Add(Lab_Name);
	option->SetName(_T("option1"));
	option->SetNormalImage(_T("file='Downloader/quanxuan.png'"));
	option->SetSelectedImage(_T("file='Downloader/quanxuanzhuangtai.png'"));
	option->SetAttribute(_T("width"), _T("22"));
	option->SetAttribute(_T("padding"), _T("20,4,2,4"));
	Lab_Name->SetText(_T("123412"));
	Lab_Name->SetFixedWidth(100);
	
	Lab_Name->SetAttribute(_T("font"), _T("2"));
	Lab_Name->SetAttribute(_T("align"), _T("center"));


}


