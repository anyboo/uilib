#include "stdafx.h"
#include "LogUI.h"

#include "CommDlg.h"

CLogUI::CLogUI()
{
}

CLogUI::~CLogUI()
{
}

DUI_BEGIN_MESSAGE_MAP(CLogUI, WindowImplBase)
DUI_ON_CLICK_CTRNAME(BT_SearchLog, OnBeginSearch)
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
	CListHeaderItemUI* Item1 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item1")));
	CListHeaderItemUI* Item2 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item2")));
	CListHeaderItemUI* Item3 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item3")));
	CListHeaderItemUI* Item4 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item4")));
	CListHeaderItemUI* Item5 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item5")));
	CListHeaderItemUI* Item6 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item6")));
	CListHeaderItemUI* Item7 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item7")));
	Item1->SetText(_T("ʱ��"));
	Item1->SetFixedWidth(200);
	Item2->SetText(_T("����"));
	Item2->SetFixedWidth(300);
	Item3->SetText(_T("����"));
	Item3->SetFixedWidth(480);
	Item4->SetFixedWidth(0);
	Item5->SetFixedWidth(0);
	Item6->SetFixedWidth(0);
	Item7->SetFixedWidth(0);
}

void CLogUI::CreateCaseList()
{
	//create case list
	CListHeaderItemUI* Item1 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item1")));
	CListHeaderItemUI* Item2 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item2")));
	CListHeaderItemUI* Item3 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item3")));
	CListHeaderItemUI* Item4 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item4")));
	CListHeaderItemUI* Item5 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item5")));
	CListHeaderItemUI* Item6 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item6")));
	CListHeaderItemUI* Item7 = static_cast<CListHeaderItemUI*>(m_PaintManager.FindControl(_T("Item7")));
	Item1->SetText(_T("ʱ��"));
	Item1->SetFixedWidth(150);
	Item2->SetText(_T("�ص�"));
	Item2->SetFixedWidth(100);
	Item3->SetText(_T("�ɼ���"));
	Item3->SetFixedWidth(100);
	Item4->SetText(_T("��������"));
	Item4->SetFixedWidth(150);
	Item5->SetText(_T("��������"));
	Item5->SetFixedWidth(150);
	Item6->SetText(_T("�豸IP"));
	Item6->SetFixedWidth(150);
	Item7->SetText(_T("����"));
	Item7->SetFixedWidth(180);
}

void CLogUI::InsertLogInfoToList()
{

}

void CLogUI::InsertCaseInfoToList()
{

}


void CLogUI::exportLog()
{
	TCHAR szBuffer[MAX_PATH] = { 0 };
	 
	OPENFILENAME  ofn = { 0 };
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = _T("TXT�ļ�(*.txt)\0*.txt\0�����ļ�(*.*)\0*.*\0");//Ҫѡ����ļ���׺   
	ofn.lpstrInitialDir = _T("D:\\");//Ĭ�ϵ��ļ�·��   
	ofn.lpstrFile = szBuffer;//����ļ��Ļ�����   
	ofn.nMaxFile = sizeof(szBuffer) / sizeof(*szBuffer);
	ofn.nFilterIndex = 0;
	ofn.Flags = OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT ;//��־����Ƕ�ѡҪ����OFN_ALLOWMULTISELECT  
	BOOL bSel = GetOpenFileName(&ofn);

}