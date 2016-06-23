#include "stdafx.h"
#include "DownLoadWnd.h"
#include "CalendarUI.h"
#include "VideoLoginUI.h"
#include "SearchFileUI.h"

DownLoadWnd::DownLoadWnd()
{
}


DownLoadWnd::~DownLoadWnd()
{
}


DUI_BEGIN_MESSAGE_MAP(DownLoadWnd, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_OnVideoLoginUI, OnVideoLoginWnd)
DUI_END_MESSAGE_MAP()

STDSTRING DownLoadWnd::intToString(int num)
{
	char Tmp[10] = { 0 };
	_itoa(num, Tmp, 10);
	STDSTRING result(Tmp);
	return result;
}

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

void DownLoadWnd::OnSearchFileWnd()
{
	SearchFileUI* pDlg = new SearchFileUI();
	assert(pDlg);
	pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_EX_DIALOG, 0L, 0, 0, 1024, 600);
	pDlg->CenterWindow();
	pDlg->ShowModal();
}

void DownLoadWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_VALUECHANGED && msg.pSender->GetName() == _T("Select_time")){
		OnSelectTimeType();
	}
	if (msg.sType == DUI_MSGTYPE_ITEMCLICK){
		INT a = 0;
	}
	if (msg.sType == DUI_MSGTYPE_CLICK){
		STDSTRING SendName = msg.pSender->GetName();
		if (msg.pSender->GetName() == BT_Calendar1 || msg.pSender->GetName() == BT_Calendar2){
			OnSelectCalendar();
		}
		if (msg.pSender->GetName() == _T("Search")){
			OnSearchFileWnd();
		}
		if (msg.pSender->GetName() == _T("All"))
		{
			CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("DownloadList")));
			COptionUI* option = static_cast<COptionUI*>(m_PaintManager.FindSubControlByName(pList, _T("option1")));
			option->Selected(true);
		}
		if (msg.pSender->GetName() == _T("test"))
		{
			if (SearchFiles()){
				ShowFileList();
			}
		}
		if (!SendName.compare(0, 9, STDSTRING(_T("BT_Cancel"))))
		{
			CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("DownloadList")));
			STDSTRING Serial = SendName.substr(9);
			int index = atoi(Serial.c_str()) - 1;
			pList->RemoveAt(index, true);
		}
	}
	WindowImplBase::Notify(msg);
}


BOOL DownLoadWnd::SearchFiles()
{
	return true;
	//m_fileInfo.clear();
	//search files
}

void DownLoadWnd::ShowFileList()
{
	CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("DownloadList")));
	//pList->RemoveAll();
	CListContainerElementUI* SubList = new CListContainerElementUI;
	int filesize = 10;
	for (int i = 1; i <= filesize; i++)
	{
		SubList = Add_FileInfoList(i);
		pList->Add(SubList);
	}

}

CListContainerElementUI* DownLoadWnd::Add_FileInfoList(int n)
{
	CListContainerElementUI* Sublist = new CListContainerElementUI;
	CHorizontalLayoutUI* hLyt = new CHorizontalLayoutUI;
	
	CLabelUI* Lab_Name = new CLabelUI;
	CLabelUI* Lab_Size = new CLabelUI;
	CProgressUI* Pro_Download = new CProgressUI;
	CLabelUI* Lab_Speed = new CLabelUI;
	CLabelUI* Lab_LastTime = new CLabelUI;
	CLabelUI* Lab_State = new CLabelUI;
	CButtonUI* BT_Cancel = new CButtonUI;

	Sublist->SetFixedHeight(30);
	Sublist->Add(hLyt);
	
	hLyt->Add(Lab_Name);
	hLyt->Add(Lab_Size);
	hLyt->Add(Pro_Download);
	hLyt->Add(Lab_Speed);
	hLyt->Add(Lab_LastTime);
	hLyt->Add(Lab_State);
	hLyt->Add(BT_Cancel);

	Lab_Name->SetAttributeList("width=\"150\" align=\"center\" font=\"2\"");
	Lab_Name->SetText(_T("name"));
	Lab_Size->SetAttributeList("width=\"100\" align=\"center\" font=\"2\"");
	Lab_Size->SetText(_T("size"));

	STDSTRING ProgressName = STDSTRING(_T("progress")) + intToString(n);
	Pro_Download->SetAttributeList(_T("width=\"140\" height=\"12\" padding=\"10,8,10,8\" font=\"3\" bordersize=\"1\" bordercolor=\"0xf1234567\" foreimage=\"file='Downloader/jindutiao.png'\""));
	Pro_Download->SetValue(50);
	Pro_Download->SetText(_T("50%"));
	Pro_Download->SetName(ProgressName.c_str());


	Lab_Speed->SetAttributeList("width=\"100\" align=\"center\" font=\"2\"");
	Lab_Speed->SetText(_T("speed"));
	Lab_LastTime->SetAttributeList("width=\"100\" align=\"center\" font=\"2\"");
	Lab_LastTime->SetText(_T("LastTime"));
	Lab_State->SetAttributeList("width=\"100\" align=\"center\" font=\"2\"");
	Lab_State->SetText(_T("State"));

	STDSTRING buttonName = STDSTRING(_T("BT_Cancel")) + intToString(n);
	BT_Cancel->SetAttributeList(_T("width=\"30\" height=\"20\" padding=\"35,5,35,5\" normalimage=\"file='Downloader/hot_del.png' dest='8,3,22,17'\" hotimage=\"file='Downloader/del_download.png' dest='8,3,22,17'\""));
	BT_Cancel->SetName(buttonName.c_str());

	return Sublist;
}
