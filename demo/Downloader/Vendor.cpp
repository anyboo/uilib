#include "stdafx.h"
#include "Vendor.h"


CVendor::CVendor()
:ppm(nullptr)
{	
}

CVendor::~CVendor()
{
}

DUI_BEGIN_MESSAGE_MAP(CVendor, CNotifyPump)

DUI_END_MESSAGE_MAP()

void CVendor::SetPaintMagager(CPaintManagerUI* pPaintMgr)
{
	assert(pPaintMgr);
	ppm = pPaintMgr;
}

void CVendor::AddVendorList()
{
	CListUI* pList = static_cast<CListUI*>(ppm->FindControl(_T("VendorList")));
	CListContainerElementUI* SubContList = new CListContainerElementUI;
	
	
	SubContList = ShowVendor(true);
	pList->Add(SubContList);
}

CListContainerElementUI* CVendor::ShowVendor(bool IsOnLine)
{
	CListContainerElementUI* VendorList = new CListContainerElementUI;
	CHorizontalLayoutUI* hLyt = new CHorizontalLayoutUI;

	CControlUI* VendorIcon = new CControlUI;
	CVerticalLayoutUI* cLyt = new CVerticalLayoutUI;
	CLabelUI* VendorName = new CLabelUI;
	CLabelUI* VendorIP = new CLabelUI;
	CControlUI* OpenIcon = new CControlUI;
	CButtonUI* BT_Delete = new CButtonUI;

	VendorList->Add(hLyt);
	hLyt->Add(VendorIcon);
	hLyt->Add(cLyt);
	hLyt->Add(OpenIcon);
	cLyt->Add(VendorName);
	cLyt->Add(VendorIP);
	hLyt->Add(BT_Delete);
	
	if (IsOnLine)
	{
		VendorIcon->SetAttributeList(_T("width=\"40\" bkimage=\"file = 'Downloader/online.png' dest = '10,20,38,47'\""));
		VendorList->SetAttributeList(_T("height=\"80\" width=\"200\" bordersize=\"1\" bordercolor=\"0xFFFFFFFF\""));
	}
	else
	{
		VendorIcon->SetAttributeList(_T("width=\"40\" bkimage=\"file = 'Downloader/network_offline.png' dest = '10,20,39,48'\""));
		VendorList->SetAttributeList(_T("height=\"80\" width=\"200\" bkimage=\"file = 'Downloader/tdxzanniu.png'\" bordersize=\"1\" bordercolor=\"0x12345678\""));
	}
	cLyt->SetFixedWidth(125);
	VendorName->SetAttributeList(_T("font=\"5\" padding=\"10, 30, 0, 0\" textcolor=\"#FFFFFFFF\""));
	VendorIP->SetAttributeList(_T("font=\"5\" padding=\"10, 0, 0, 0\" textcolor=\"#FFFFFFFF\""));
	VendorName->SetText(_T("³§ÉÌÃû"));
	VendorIP->SetText(_T("(192.168.0.21)"));

	OpenIcon->SetAttributeList(_T("width=\"19\" bkimage=\"file = 'Downloader/xxzk.png' dest = '0,36,19,48'\""));
	BT_Delete->SetAttributeList(_T("width=\"14\" height=\"14\" padding=\"0,2,2,0\" normalimage=\"file='Downloader/hot_del.png' dest='1,0,15,15'\" hotimage=\"file='Downloader/del_download.png' dest='1,0,15,15'\""));
	return VendorList;
}