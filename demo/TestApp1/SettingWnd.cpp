#include "stdafx.h"
#include "SettingWnd.h"
#include "Setting.h"

CSettingWnd::CSettingWnd()
{
}

CSettingWnd::~CSettingWnd()
{
}

void CSettingWnd::InitWindow()
{
	label = dynamic_cast<CLabelUI*>(m_PaintManager.FindControl(_T("edit")));
	assert(label);
	if (!label) return;
	std::wstring location;
	CSetting::Inst().GetLocation(location);
	label->SetText(location.c_str());

	combo = dynamic_cast<CComboUI*>(m_PaintManager.FindControl(_T("encoding")));
	assert(combo);
	if (!combo) return;
	combo->SelectItem(0);
}

void CSettingWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	//delete this;
}

void CSettingWnd::Notify(TNotifyUI& msg)
{
	trace(msg);
	DUITRACE(_T("VirtualWnd: %s"), msg.sVirtualWnd);
	WindowImplBase::Notify(msg);
}


DUI_BEGIN_MESSAGE_MAP(CSettingWnd, WindowImplBase)
DUI_ON_CLICK_CTRNAME(CTR_FOLDER, OnSelectFolder)
DUI_ON_MSGTYPE_CTRNAME(DUI_MSGTYPE_ITEMSELECT, CTR_ENCODING, OnSelectChanged)
DUI_END_MESSAGE_MAP()

LPCTSTR CSettingWnd::GetWindowClassName() const
{
	return _T("SettingWnd");
}

CDuiString CSettingWnd::GetSkinFolder()
{
	return _T("skin\\");
}

CDuiString CSettingWnd::GetSkinFile()
{
	return _T("SettingDlg.xml");
}

void CSettingWnd::OnSelectChanged(TNotifyUI& msg)
{
	int sel = combo->GetCurSel();
	CDuiString str = combo->GetText();
	DUITRACE(_T("combo->GetText : %s index : %d"), str, sel);
	CSetting::Inst().SetEncode(str.GetData());
}

#include <Objbase.h>
#include <Shobjidl.h>

//fixed later,maybe resource leak.
void CSettingWnd::OnSelectFolder(TNotifyUI& msg)
{
	IFileOpenDialog* pfd;
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pfd));

	if (FAILED(hr)) return;
	DWORD dwOptions;
	hr = pfd->GetOptions(&dwOptions);
	if (FAILED(hr)) return;
	hr = pfd->SetOptions(dwOptions | FOS_PICKFOLDERS | FOS_PATHMUSTEXIST);
	if (FAILED(hr)) return;
	hr = pfd->Show(NULL);
	if (FAILED(hr)) return;
	IShellItemArray *psiaResults;
	hr = pfd->GetResults(&psiaResults);
	DWORD conut = 0;
	hr = psiaResults->GetCount(&conut);
	
	LPWSTR NAME, DISPLAYNAME;
	IShellItem *item;
	psiaResults->GetItemAt(0, &item);
	hr = item->GetDisplayName(SIGDN_DESKTOPABSOLUTEEDITING, &DISPLAYNAME);
	hr = item->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &NAME); 

	psiaResults->Release();
	pfd->Release();

	DUITRACE(_T("SELECTED FOLDER : %s"), DISPLAYNAME);	
	CSetting::Inst().SetLocation(NAME);

	label->SetText(DISPLAYNAME);
}