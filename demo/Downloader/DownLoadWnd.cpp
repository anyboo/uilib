#include "stdafx.h"
#include "DownLoadWnd.h"
#include "CalendarUI.h"
#include "VideoLoginUI.h"
#include "SearchFileUI.h"

DownLoadWnd::DownLoadWnd()
:m_FileCount(0)
{
	m_Vendor.SetPaintMagager(&m_PaintManager);
	AddVirtualWnd(_T("Vendor"), &m_Vendor);
}


DownLoadWnd::~DownLoadWnd()
{
	RemoveVirtualWnd(_T("Vendor"));
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
	pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_EX_DIALOG, 0L, 0, 0, 0, 0);
	pDlg->CenterWindow();
	pDlg->ShowModal();
}

void DownLoadWnd::OnVideoLoginWnd(TNotifyUI& msg)
{
	VideoLoginUI* pDlg = new VideoLoginUI();
	assert(pDlg);
	pDlg->Create(this->GetHWND(), NULL, UI_WNDSTYLE_CONTAINER, 0L, 1024, 768, 0, 0);
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
	STDSTRING strSendName = msg.pSender->GetName();
	if (msg.sType == DUI_MSGTYPE_VALUECHANGED && strSendName == _T("Select_time")){
		OnSelectTimeType();
	}
	if (msg.sType == DUI_MSGTYPE_ITEMCLICK && !strSendName.compare(0, SUBLISTNAMELONG, SUBLISTNAMETAG)){
		Show_Off_SubList(strSendName);
	}
	if (msg.sType == DUI_MSGTYPE_CLICK){
		if (strSendName == BT_Calendar1 || strSendName == BT_Calendar2){
			OnSelectCalendar();
		}
		if (strSendName == _T("Search")){
			OnSearchFileWnd();
		}
		if (strSendName == _T("test"))
		{	
			m_Vendor.AddVendorList();
			
		}
		if (strSendName == _T("test2"))
		{
			ShowFileList();
		}
		if (!strSendName.compare(0, BTNAMELONG, BTNAMETAG))
		{
			RemoveSubList(strSendName);
		}
	}
	if (msg.sType == DUI_MSGTYPE_ITEMCLICK && !strSendName.compare(0, 14, _T("VendorContList")))
	{
		Show_Off_VendorList(strSendName);
	}
	if (msg.sType == DUI_MSGTYPE_CLICK && strSendName == _T("quanxuan"))
	{
		All_SelectChannels();
	}
	if (msg.sType == DUI_MSGTYPE_CLICK && !strSendName.compare(0, 9, _T("BT_delete")))
	{
		RemoveVendor(strSendName);
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

	SubList = Add_FileInfoList(1, true);
	pList->Add(SubList);
}

CListContainerElementUI* DownLoadWnd::Add_FileInfoList(int n, bool IsShowCloseBT)
{
	m_FileCount = m_FileCount + n;
	CListContainerElementUI* Sublist = new CListContainerElementUI;
	CHorizontalLayoutUI* hLyt = new CHorizontalLayoutUI;

	CLabelUI* Lab_Name = new CLabelUI;
	CLabelUI* Lab_Size = new CLabelUI;
	CProgressUI* Pro_Download = new CProgressUI;
	CLabelUI* Lab_Speed = new CLabelUI;
	CLabelUI* Lab_LastTime = new CLabelUI;
	CLabelUI* Lab_State = new CLabelUI;
	CButtonUI* BT_Cancel = new CButtonUI;
	CTreeViewUI* sas = new CTreeViewUI;
	
	if (IsShowCloseBT){
		STDSTRING SublistName = SUBLISTNAMETAG + intToString(m_FileCount);
		Sublist->SetUserData(_T("0"));
		Sublist->SetName(SublistName.c_str());
	}
	Sublist->SetFixedHeight(30);
	Sublist->Add(hLyt);
	hLyt->Add(Lab_Name);
	hLyt->Add(Lab_Size);
	hLyt->Add(Pro_Download);
	hLyt->Add(Lab_Speed);
	hLyt->Add(Lab_LastTime);
	hLyt->Add(Lab_State);
	hLyt->Add(BT_Cancel);

	if (!IsShowCloseBT)
	{
		Lab_Name->SetAttribute(_T("padding"), _T("30,0,0,0"));
	}
	Lab_Name->SetAttributeList("width=\"150\" align=\"center\" font=\"2\"");
	Lab_Name->SetText(_T("name"));
	Lab_Size->SetAttributeList("width=\"100\" align=\"center\" font=\"2\"");
	Lab_Size->SetText(_T("size"));

	STDSTRING ProgressName = STDSTRING(_T("progress")) + intToString(m_FileCount);
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

	STDSTRING buttonName = STDSTRING(_T("BT_Cancel")) + intToString(m_FileCount);
	BT_Cancel->SetAttributeList(_T("width=\"30\" height=\"20\" padding=\"35,5,35,5\" normalimage=\"file='Downloader/hot_del.png' dest='8,3,22,17'\" hotimage=\"file='Downloader/del_download.png' dest='8,3,22,17'\""));
	BT_Cancel->SetName(buttonName.c_str());
	BT_Cancel->SetVisible(IsShowCloseBT);

	return Sublist;
}

int DownLoadWnd::GetSubListCurSel(CListContainerElementUI* SubList, CListUI* pList)
{

	CListContainerElementUI* SubListTmp = new CListContainerElementUI;
	int CurSel = -1;
	for (int i = 0; i < pList->GetCount(); i++)
	{
		SubListTmp = static_cast<CListContainerElementUI*>(m_PaintManager.FindSubControlByClass(pList, DUI_CTR_LISTCONTAINERELEMENT, i));
		if (SubListTmp->GetName() == SubList->GetName())
		{
			CurSel = i;
			break;
		}
	}
	return CurSel;
}

void DownLoadWnd::Show_Off_SubList(STDSTRING& strSendName)
{
	CListUI* m_List = static_cast<CListUI*>(m_PaintManager.FindControl(_T("DownloadList")));

	int filesize = 5;
	STDSTRING strUserData;
	CListContainerElementUI* ContList = static_cast<CListContainerElementUI*>(m_PaintManager.FindSubControlByName(m_List, strSendName.c_str()));
	int CurSel = GetSubListCurSel(ContList, m_List);
	CListContainerElementUI* SubContList = static_cast<CListContainerElementUI*>(m_PaintManager.FindSubControlByClass(m_List, DUI_CTR_LISTCONTAINERELEMENT, CurSel + 1));
	if (SubContList == NULL)
	{
		if (ContList->GetUserData() == _T("0"))
		{
			CListContainerElementUI* SubList = new CListContainerElementUI;
			for (int i = CurSel + 1; i <= CurSel + filesize; i++)
			{
				SubList = Add_FileInfoList(i, false);
				SubList->SetUserData(_T("Sub"));
				m_List->AddAt(SubList, i);
			}
			strUserData = intToString(filesize);
			ContList->SetUserData(strUserData.c_str());
		}
	}
	else{
		if (ContList->GetUserData() == _T("0") && SubContList->GetUserData() != _T("Sub"))
		{
			CListContainerElementUI* SubList = new CListContainerElementUI;
			for (int i = CurSel + 1; i <= CurSel + filesize; i++)
			{
				SubList = Add_FileInfoList(i, false);
				SubList->SetUserData(_T("Sub"));
				m_List->AddAt(SubList, i);
			}
			strUserData = intToString(filesize);
			ContList->SetUserData(strUserData.c_str());
		}
		if (ContList->GetUserData() != _T("0") && SubContList->GetUserData() == _T("Sub"))
		{
			strUserData = ContList->GetUserData();
			int Count = atoi(strUserData.c_str());
			for (int j = CurSel + 1; j <= CurSel + Count; j++)
			{
				m_List->RemoveAt(CurSel + 1, false);
			}
			ContList->SetUserData(_T("0"));
		}
	}
}

void DownLoadWnd::RemoveSubList(STDSTRING& strSendName)
{
	CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("DownloadList")));
	STDSTRING Serial = strSendName.substr(BTNAMELONG);
	STDSTRING ContListName = SUBLISTNAMETAG + Serial;
	CListContainerElementUI* ContList = static_cast<CListContainerElementUI*>(m_PaintManager.FindSubControlByName(pList, ContListName.c_str()));
	int ContListserial = GetSubListCurSel(ContList, pList);
	STDSTRING SubListCount = ContList->GetUserData();
	int Count = atoi(SubListCount.c_str());
	for (int i = 0; i <= Count; i++)
	{
		pList->RemoveAt(ContListserial, true);
	}
}

void DownLoadWnd::Show_Off_VendorList(STDSTRING& strSendName)
{
	CListUI* VendorList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("VendorList")));
	CListContainerElementUI* Channel_List = static_cast<CListContainerElementUI*>(m_PaintManager.FindSubControlByName(VendorList, _T("Channel_List")));
	CListContainerElementUI* CurSelList = static_cast<CListContainerElementUI*>(m_PaintManager.FindSubControlByName(VendorList, strSendName.c_str()));
	int CurSel = GetSubListCurSel(CurSelList, VendorList);
	if (Channel_List == NULL)
	{
		m_Vendor.AddChannelsList(CurSel);
	}
	else
	{
		int Channel_List_CurSel = GetSubListCurSel(Channel_List, VendorList);
		VendorList->RemoveAt(Channel_List_CurSel, true);
		if (Channel_List_CurSel != CurSel + 1 && Channel_List_CurSel > CurSel)
		{
			m_Vendor.AddChannelsList(CurSel);
		}
		else if (Channel_List_CurSel != CurSel + 1 && Channel_List_CurSel < CurSel)
		{
			m_Vendor.AddChannelsList(CurSel - 1);
		}
	}
}

void DownLoadWnd::All_SelectChannels()
{
	CListUI* VendorList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("VendorList")));
	COptionUI* option_All = static_cast<COptionUI*>(m_PaintManager.FindSubControlByName(VendorList, _T("quanxuan")));
	CDuiPtrArray* array = m_PaintManager.FindSubControlsByClass(VendorList, DUI_CTR_OPTION);
	int option_size = array->GetSize();
	for (int i = 1; i < option_size; i++)
	{
		COptionUI* option = static_cast<COptionUI*>(m_PaintManager.FindSubControlByClass(VendorList, DUI_CTR_OPTION, i));
		if (!option_All->IsSelected()){
			option->Selected(true);
		}
		else{
			option->Selected(false);
		}
	}
}

void DownLoadWnd::RemoveVendor(STDSTRING& strSendName)
{
	CListUI* VendorList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("VendorList")));
	CButtonUI* BT_delete = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(VendorList, strSendName.c_str()));
	STDSTRING Serial = strSendName.substr(9);
	STDSTRING ContListName = STDSTRING(_T("VendorContList")) + Serial;
	CListContainerElementUI* CurSelList = static_cast<CListContainerElementUI*>(m_PaintManager.FindSubControlByName(VendorList, ContListName.c_str()));
	int CurSel = GetSubListCurSel(CurSelList, VendorList);
	CListContainerElementUI* NextList = static_cast<CListContainerElementUI*>(m_PaintManager.FindSubControlByClass(VendorList, DUI_CTR_LISTCONTAINERELEMENT, CurSel + 1));
	if (NextList != NULL && NextList->GetName() == _T("Channel_List"))
	{
		VendorList->RemoveAt(CurSel + 1, true);
	}
	VendorList->RemoveAt(CurSel, true);
}