#include "stdafx.h"
#include "LogUI.h"
#include "CalendarUI.h"
#include "CommDlg.h"

CLogUI::CLogUI()
{
	m_bInit = TRUE;
}

CLogUI::~CLogUI()
{
}

DUI_BEGIN_MESSAGE_MAP(CLogUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_SearchLog, OnBeginSearch)
DUI_ON_CLICK_CTRNAME(BT_OnStartTime, OnSelectStartTime)
DUI_ON_CLICK_CTRNAME(BT_OnStopTime, OnSelectStopTime)
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
	if (m_bInit)
	{
		m_pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("domainlist")));
		m_bInit = FALSE;
	}
	if (msg.sType == DUI_MSGTYPE_CLICK)	{
		if (msg.pSender->GetName() == _T("LogType1")){
			CreateLogList();
		}
		if (msg.pSender->GetName() == _T("LogType2")){
			CreateCaseList();
		}
		if (msg.pSender->GetName() == _T("export")){
			exportLog();
		}
	}
	WindowImplBase::Notify(msg);
}

void CLogUI::OnBeginSearch(TNotifyUI& msg)
{
	COptionUI* Option1 = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("LogType1")));
	if (Option1->IsSelected())
	{
		OnSearchLog();
		InsertLogInfoToList();
	}
	else
	{
		OnSearchCaseLog();
		InsertCaseInfoToList();
	}
}

void CLogUI::OnSearchLog()
{
	m_LogInfo.clear();
	//Search Log infomaton
}

void CLogUI::OnSearchCaseLog()
{
	m_CaseInfo.clear();
	//Search case information
}

void CLogUI::CreateLogList()
{
	//create log list
	m_pList->RemoveAll();
	CListHeaderUI* list_head = static_cast<CListHeaderUI*>(m_PaintManager.FindControl(_T("list_head")));
	CListHeaderUI* newlist = new CListHeaderUI;
	newlist->SetName(_T("list_head"));
	CListHeaderItemUI* Item1 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item1")));
	CListHeaderItemUI* Item2 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item2")));
	CListHeaderItemUI* Item3 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item3")));
	CListHeaderItemUI* Item4 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item4")));
	CListHeaderItemUI* Item5 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item5")));
	CListHeaderItemUI* Item6 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item6")));
	CListHeaderItemUI* Item7 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item7")));
	CListHeaderItemUI* item1 = new CListHeaderItemUI;
	CListHeaderItemUI* item2 = new CListHeaderItemUI;
	CListHeaderItemUI* item3 = new CListHeaderItemUI;
	CListHeaderItemUI* item4 = new CListHeaderItemUI;
	CListHeaderItemUI* item5 = new CListHeaderItemUI;
	CListHeaderItemUI* item6 = new CListHeaderItemUI;
	CListHeaderItemUI* item7 = new CListHeaderItemUI;
	item1->SetName(_T("Item1"));
	item1->SetAttribute(_T("font"), _T("1"));
	item2->SetName(_T("Item2"));
	item2->SetAttribute(_T("font"), _T("1"));
	item3->SetName(_T("Item3"));
	item3->SetAttribute(_T("font"), _T("1"));
	item4->SetName(_T("Item4"));
	item5->SetName(_T("Item5"));
	item6->SetName(_T("Item6"));
	item7->SetName(_T("Item7"));
	item1->SetText(_T("时间"));
	item1->SetFixedWidth(200);
	item2->SetText(_T("操作"));
	item2->SetFixedWidth(300);
	item3->SetText(_T("描述"));
	item3->SetFixedWidth(480);
	item4->SetFixedWidth(0);
	item5->SetFixedWidth(0);
	item6->SetFixedWidth(0);
	item7->SetFixedWidth(0);
	list_head->Remove(Item1);
	list_head->Remove(Item2);
	list_head->Remove(Item3);
	list_head->Remove(Item4);
	list_head->Remove(Item5);
	list_head->Remove(Item6);
	list_head->Remove(Item7);
	m_pList->Remove(list_head);
	m_pList->Add(newlist);
	m_pList->SetAttribute(_T("headerbkimage"), _T("file='Downloader/lbl.png'"));
	newlist->SetFixedHeight(31);
	newlist->Add(item1);
	newlist->Add(item2);
	newlist->Add(item3);
	newlist->Add(item4);
	newlist->Add(item5);
	newlist->Add(item6);
	newlist->Add(item7);
	
}

void CLogUI::CreateCaseList()
{
	//create case list
	m_pList->RemoveAll();
	
	CListHeaderUI* list_head = static_cast<CListHeaderUI*>(m_PaintManager.FindControl(_T("list_head")));
	CListHeaderUI* newlist = new CListHeaderUI;
	newlist->SetName(_T("list_head"));
	CListHeaderItemUI* Item1 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item1")));
	CListHeaderItemUI* Item2 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item2")));
	CListHeaderItemUI* Item3 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item3")));
	CListHeaderItemUI* Item4 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item4")));
	CListHeaderItemUI* Item5 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item5")));
	CListHeaderItemUI* Item6 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item6")));
	CListHeaderItemUI* Item7 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item7")));
	CListHeaderItemUI* item1 = new CListHeaderItemUI;
	CListHeaderItemUI* item2 = new CListHeaderItemUI;
	CListHeaderItemUI* item3 = new CListHeaderItemUI;
	CListHeaderItemUI* item4 = new CListHeaderItemUI;
	CListHeaderItemUI* item5 = new CListHeaderItemUI;
	CListHeaderItemUI* item6 = new CListHeaderItemUI;
	CListHeaderItemUI* item7 = new CListHeaderItemUI;
	item1->SetName(_T("Item1"));
	item1->SetAttribute(_T("font"), _T("1"));
	item2->SetName(_T("Item2"));
	item2->SetAttribute(_T("font"), _T("1"));
	item3->SetName(_T("Item3"));
	item3->SetAttribute(_T("font"), _T("1"));
	item4->SetName(_T("Item4"));
	item4->SetAttribute(_T("font"), _T("1"));
	item5->SetName(_T("Item5"));
	item5->SetAttribute(_T("font"), _T("1"));
	item6->SetName(_T("Item6"));
	item6->SetAttribute(_T("font"), _T("1"));
	item7->SetName(_T("Item7"));
	item7->SetAttribute(_T("font"), _T("1"));

	item1->SetText(_T("时间"));
	item1->SetFixedWidth(150);
	item2->SetText(_T("地点"));
	item2->SetFixedWidth(100);
	item3->SetText(_T("采集人"));
	item3->SetFixedWidth(100);
	item4->SetText(_T("案件名称"));
	item4->SetFixedWidth(150);
	item5->SetText(_T("案件描述"));
	item5->SetFixedWidth(150);
	item6->SetText(_T("设备IP"));
	item6->SetFixedWidth(150);
	item7->SetText(_T("描述"));
	item7->SetFixedWidth(180);
	list_head->Remove(Item1);
	list_head->Remove(Item2);
	list_head->Remove(Item3);
	list_head->Remove(Item4);
	list_head->Remove(Item5);
	list_head->Remove(Item6);
	list_head->Remove(Item7);
	m_pList->Remove(list_head);
	m_pList->Add(newlist);
	m_pList->SetAttribute(_T("headerbkimage"), _T("file='Downloader/lbl.png'"));
	newlist->SetFixedHeight(31);
	newlist->Add(item1);
	newlist->Add(item2);
	newlist->Add(item3);
	newlist->Add(item4);
	newlist->Add(item5);
	newlist->Add(item6);
	newlist->Add(item7);
}

void CLogUI::InsertLogInfoToList()
{
	//m_pList->RemoveAll();
	for (int i = 0; i < 10; i++)
	{
		CListTextElementUI* pListElement = new CListTextElementUI;
		pListElement->SetTag(i);
		m_pList->Add(pListElement);
		pListElement->SetAttribute(_T("font"), _T("1"));
		pListElement->SetFixedHeight(30);
		pListElement->SetText(0, _T("WHO1753"));
		pListElement->SetText(1, _T("程序设计"));
		pListElement->SetText(2, _T("100"));		
	}
	
	//for (UINT i = 0; i < m_LogInfo.size(); i++)
	//{
	//	CListTextElementUI* pListElement = new CListTextElementUI;
	//	pListElement->SetTag(i);
	//	m_pList->Add(pListElement);
	//	pListElement->SetFixedHeight(30);
	//	pListElement->SetText(0, m_LogInfo[i].Time.c_str());
	//	pListElement->SetText(1, m_LogInfo[i].Handle.c_str());
	//	pListElement->SetText(2, m_LogInfo[i].Description.c_str());
	//}
}

void CLogUI::InsertCaseInfoToList()
{
	m_pList->RemoveAll();
	TListInfoUI* List_Info =  m_pList->GetListInfo();
	for (int i = 0; i < 10; i++)
	{
		CListTextElementUI* pListElement = new CListTextElementUI;
		pListElement->SetTag(i);
		m_pList->Add(pListElement);
		pListElement->SetFixedHeight(30);
		pListElement->SetText(0, _T("WHO1753"));
		pListElement->SetText(1, _T("程序设计"));
		pListElement->SetText(2, _T("100"));
		pListElement->SetText(3, _T("WHO1753"));
		pListElement->SetText(4, _T("程序设计"));
		pListElement->SetText(5, _T("100"));
		pListElement->SetText(6, _T("100"));
	}
}


void CLogUI::exportLog()
{
	TCHAR szBuffer[MAX_PATH] = { 0 };
	 
	OPENFILENAME  ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = _T("TXT文件(*.txt)\0*.txt\0所有文件(*.*)\0*.*\0");//要选择的文件后缀   
	ofn.lpstrInitialDir = _T("D:\\");//默认的文件路径   
	ofn.lpstrFile = szBuffer;//存放文件的缓冲区   
	ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT ;//标志如果是多选要加上OFN_ALLOWMULTISELECT  
	BOOL bSel = GetOpenFileName(&ofn);

}

void CLogUI::OnSelectStartTime(TNotifyUI& msg)
{
	CalendarUI* pDlg = new CalendarUI();
	assert(pDlg);
	pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_EX_DIALOG, 0L, 0, 0, 350, 380);
	pDlg->CenterWindow();
	pDlg->ShowModal();
}

void CLogUI::OnSelectStopTime(TNotifyUI& msg)
{
	CalendarUI* pDlg = new CalendarUI();
	assert(pDlg);
	pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_EX_DIALOG, 0L, 0, 0, 350, 380);
	pDlg->CenterWindow();
	pDlg->ShowModal();
}