#include "stdafx.h"
#include "Vendor.h"


CVendor::CVendor()
:ppm(nullptr), m_ContListSel(0)
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

STDSTRING CVendor::intToString(int num)
{
	char Tmp[10] = { 0 };
	_itoa(num, Tmp, 10);
	STDSTRING result(Tmp);
	return result;
}

void CVendor::AddChannelsList(int CurSel)
{
	CListUI* pList = static_cast<CListUI*>(ppm->FindControl(_T("VendorList")));
	CListContainerElementUI* SubContList = new CListContainerElementUI;

	SubContList = AddChannels();
	pList->AddAt(SubContList, CurSel + 1);

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
	
	m_ContListSel++;
	STDSTRING strContListName = STDSTRING(_T("VendorContList")) + intToString(m_ContListSel);
	VendorList->SetName(strContListName.c_str());

	if (IsOnLine)
	{
		VendorIcon->SetAttributeList(_T("width=\"40\" bkimage=\"file = 'Downloader/online.png' dest = '10,20,38,47'\""));
		VendorList->SetAttributeList(_T("height=\"80\" width=\"185\" bordersize=\"1\" bordercolor=\"0xFFFFFFFF\""));
	}
	else
	{
		VendorIcon->SetAttributeList(_T("width=\"40\" bkimage=\"file = 'Downloader/network_offline.png' dest = '10,20,39,48'\""));
		VendorList->SetAttributeList(_T("height=\"80\" width=\"200\" bkimage=\"file = 'Downloader/tdxzanniu.png'\" bordersize=\"1\" bordercolor=\"0x12345678\""));
	}
	cLyt->SetFixedWidth(110);
	VendorName->SetAttributeList(_T("font=\"5\" padding=\"10, 30, 0, 0\" textcolor=\"#FFFFFFFF\""));
	VendorIP->SetAttributeList(_T("font=\"4\" padding=\"10, 0, 0, 0\" textcolor=\"#FFFFFFFF\""));
	VendorName->SetText(_T("厂商名"));
	VendorIP->SetText(_T("(192.168.0.21)"));

	OpenIcon->SetAttributeList(_T("width=\"19\" bkimage=\"file = 'Downloader/xxzk.png' dest = '0,36,19,48'\""));
	BT_Delete->SetAttributeList(_T("width=\"14\" height=\"14\" padding=\"0,2,2,0\" normalimage=\"file='Downloader/hot_del.png' dest='1,0,15,15'\" hotimage=\"file='Downloader/del_download.png' dest='1,0,15,15'\""));
	STDSTRING strBtName = STDSTRING(_T("BT_delete")) + intToString(m_ContListSel);
	BT_Delete->SetName(strBtName.c_str());

	return VendorList;
}

CListContainerElementUI* CVendor::AddChannels()
{
	CListContainerElementUI* ContList = new CListContainerElementUI;
	CVerticalLayoutUI* vLyt = new CVerticalLayoutUI;

	int Channel_Count = 14;

	ContList->SetName(_T("Channel_List"));
	ContList->SetFixedHeight(30 * Channel_Count + 30);
	ContList->SetFixedWidth(185);
	ContList->SetMouseEnabled(false);
	ContList->Add(vLyt);

	for (int i = 0; i <= Channel_Count; i++)
	{
		CHorizontalLayoutUI* subHlyt = new CHorizontalLayoutUI;
		COptionUI* subOption = new COptionUI;
		CLabelUI* subLab = new CLabelUI;
		vLyt->Add(subHlyt);
		subHlyt->SetFixedHeight(30);
		subHlyt->Add(subOption);
		subHlyt->Add(subLab);
		if (i == 0)
		{
			subOption->SetAttributeList(_T("name=\"quanxuan\" width=\"22\" height=\"22\" padding=\"20,4,10,4\" normalimage=\"file='Downloader/quanxuan.png'\" selectedimage=\"file='Downloader/quanxuanzhuangtai.png'\""));
			subLab->SetAttributeList(_T("text=\"全选\" font=\"5\" textcolor=\"#FFFFFFFF\" valign=\"center\""));
		}
		else{
			subOption->SetAttributeList(_T("width=\"22\" height=\"22\" padding=\"40,4,10,4\" normalimage=\"file='Downloader/quanxuan.png'\" selectedimage=\"file='Downloader/quanxuanzhuangtai.png'\""));
			subLab->SetAttributeList(_T("font=\"5\" valign=\"center\" textcolor=\"#FFFFFFFF\""));
			STDSTRING strChannelName = STDSTRING(_T("通道")) + intToString(i);
			subLab->SetText(strChannelName.c_str());
		}		
	}

	return ContList;
}