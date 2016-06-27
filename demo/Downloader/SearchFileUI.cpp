#include "stdafx.h"
#include "SearchFileUI.h"


SearchFileUI::SearchFileUI()
{
}


SearchFileUI::~SearchFileUI()
{
}

DUI_BEGIN_MESSAGE_MAP(SearchFileUI, WindowImplBase)
DUI_END_MESSAGE_MAP()

STDSTRING SearchFileUI::intToString(int num)
{
	char Tmp[10] = { 0 };
	_itoa(num, Tmp, 10);
	STDSTRING result(Tmp);
	return result;
}

LPCTSTR SearchFileUI::GetWindowClassName() const
{
	return _T("SearchFileUI");
}

CDuiString SearchFileUI::GetSkinFolder()
{
	return _T("..//x86");
}

CDuiString SearchFileUI::GetSkinFile()
{
	return _T("VideoSearch.xml");
}

void SearchFileUI::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void SearchFileUI::Notify(TNotifyUI& msg)
{	
	if (msg.sType == DUI_MSGTYPE_CLICK){
		STDSTRING SendName = msg.pSender->GetName();
		if (msg.pSender->GetName() == _T("close"))
		{
		Close();
		}
		if (msg.pSender->GetName() == _T("test"))
		{
		SearchTest();
		}
		if (msg.pSender->GetName() == _T("All"))
		{
			CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("domainlist")));
			COptionUI* option_All = static_cast<COptionUI*>(m_PaintManager.FindSubControlByName(pList, _T("All")));
			int nCount = pList->GetCount();
			for (int i = 1; i <= nCount; i++)
			{
				COptionUI* option = static_cast<COptionUI*>(m_PaintManager.FindSubControlByClass(pList, DUI_CTR_OPTION, i));
				if (!option_All->IsSelected()){
					option->Selected(true);
				}
				else{
					option->Selected(false);
				}
			}
		}
	}

	WindowImplBase::Notify(msg);
}


void SearchFileUI::SearchTest()
{
	CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("domainlist")));
//	pList->RemoveAll();
	CListContainerElementUI* SubList = new CListContainerElementUI;
	int filesize = 10;
	for (int i = 1; i <= filesize; i++)
	{
		SubList = Add_FileInfoList(i);
		pList->Add(SubList);
	}
}

CListContainerElementUI* SearchFileUI::Add_FileInfoList(int n)
{
	CListContainerElementUI* Sublist = new CListContainerElementUI;
	CHorizontalLayoutUI* hLyt = new CHorizontalLayoutUI;
	COptionUI* option = new COptionUI;
	CLabelUI* Lab_Name = new CLabelUI;
	CLabelUI* Lab_Channel = new CLabelUI;
	CLabelUI* Lab_Stime = new CLabelUI;
	CLabelUI* Lab_Etime = new CLabelUI;
	CLabelUI* Lab_Size = new CLabelUI;
	CButtonUI* BT_Play = new CButtonUI;

	Sublist->SetFixedHeight(30);
	Sublist->Add(hLyt);
	hLyt->Add(option);
	hLyt->Add(Lab_Name);
	hLyt->Add(Lab_Channel);
	hLyt->Add(Lab_Stime);
	hLyt->Add(Lab_Etime);
	hLyt->Add(Lab_Size);
	hLyt->Add(BT_Play);

	STDSTRING optionName = STDSTRING(_T("option")) + intToString(n);
	option->SetAttributeList(_T("width=\"22\" height=\"22\" padding=\"10,4,42,4\" normalimage=\"file='Downloader/quanxuan.png'\" selectedimage=\"file='Downloader/quanxuanzhuangtai.png'\""));
	option->SetName(optionName.c_str());

	Lab_Name->SetAttributeList("width=\"240\" align=\"center\" font=\"2\"");
	Lab_Name->SetText(_T("123412"));
	Lab_Channel->SetAttributeList("width=\"100\" align=\"center\" font=\"2\"");
	Lab_Channel->SetText(_T("text"));
	Lab_Stime->SetAttributeList("width=\"200\" align=\"center\" font=\"2\"");
	Lab_Stime->SetText(_T("text"));
	Lab_Etime->SetAttributeList("width=\"200\" align=\"center\" font=\"2\"");
	Lab_Etime->SetText(_T("text"));
	Lab_Size->SetAttributeList("width=\"100\" align=\"center\" font=\"2\"");
	Lab_Size->SetText(_T("text"));

	STDSTRING buttonName = STDSTRING(_T("BT_Play")) + intToString(n);
	BT_Play->SetAttributeList(_T("width=\"40\" height=\"30\" padding=\"30,0,30,0\" foreimage=\"file='Downloader/play.png' dest='5,0,35,30'\" hotimage=\"file='Downloader/Button_Hot.png'\""));
	BT_Play->SetName(buttonName.c_str());

	return Sublist;
}