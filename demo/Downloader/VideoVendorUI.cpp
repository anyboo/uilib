#include "stdafx.h"
#include "VideoVendorUI.h"


CVideoVendorUI::CVideoVendorUI()
{
}


CVideoVendorUI::~CVideoVendorUI()
{
}

DUI_BEGIN_MESSAGE_MAP(CVideoVendorUI, WindowImplBase)

DUI_END_MESSAGE_MAP()

LPCTSTR CVideoVendorUI::GetWindowClassName() const
{
	return _T("CLogUI");
}

CDuiString CVideoVendorUI::GetSkinFolder()
{
	return _T("..//x86");
}

CDuiString CVideoVendorUI::GetSkinFile()
{
	return _T("VideoVendor.xml");
}

void CVideoVendorUI::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

STDSTRING CVideoVendorUI::intToString(int num)
{
	char Tmp[10] = { 0 };
	_itoa(num, Tmp, 10);
	STDSTRING result(Tmp);
	return result;
}

void CVideoVendorUI::Notify(TNotifyUI& msg)
{
	if (msg.sType == DUI_MSGTYPE_CLICK )
	{
		if (msg.pSender->GetName() == _T("close_btn"))
		{
			Close();
		}
		if (msg.pSender->GetName() == _T("Prev_page"))
		{
			GetPrevPage();
		}
		if (msg.pSender->GetName() == _T("Next_page"))
		{
			GetNextPage();
		}
	}
	if (msg.sType == DUI_MSGTYPE_ITEMSELECT && msg.pSender->GetName() == _T("equipment_List"))
	{
		SelectEquipment();
	}
	
	WindowImplBase::Notify(msg);
}

void CVideoVendorUI::GetPrevPage()
{
	CLabelUI* Lab_Page = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("pages")));
	STDSTRING Lab_Text = Lab_Page->GetText();
	STDSTRING pages = Lab_Text.substr(0, Lab_Text.find_first_of(_T("/")));
	int page_Num = atoi(pages.c_str());
	if (page_Num > 1)
	{
		page_Num--;
	}
	Lab_Text = intToString(page_Num) + STDSTRING(_T("/20"));
	Lab_Page->SetText(Lab_Text.c_str());
}

void CVideoVendorUI::GetNextPage()
{
	CLabelUI* Lab_Page = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("pages")));
	STDSTRING Lab_Text = Lab_Page->GetText();
	STDSTRING pages = Lab_Text.substr(0, Lab_Text.find_first_of(_T("/")));
	int page_Num = atoi(pages.c_str());
	if (page_Num < 20)
	{
		page_Num++;
	}
	Lab_Text = intToString(page_Num) + STDSTRING(_T("/20"));
	Lab_Page->SetText(Lab_Text.c_str());
}

void CVideoVendorUI::SelectEquipment()
{
	CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("equipment_List")));
	int CurSel = pList->GetCurSel();
	STDSTRING SubOptionName = STDSTRING(_T("CheckBox")) + intToString(CurSel);
	COptionUI* SubOPtion = static_cast<COptionUI*>(m_PaintManager.FindControl(SubOptionName.c_str()));
	SubOPtion->Selected(true);
}