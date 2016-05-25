#include "stdafx.h"
#include "SettingWnd.h"

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
}

void CSettingWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CSettingWnd::Notify(TNotifyUI& msg)
{
	trace(msg);
	DUITRACE("VirtualWnd: %s", msg.sVirtualWnd);
	WindowImplBase::Notify(msg);
}


DUI_BEGIN_MESSAGE_MAP(CSettingWnd, WindowImplBase)
DUI_ON_CLICK_CTRNAME(CTR_FOLDER, OnSelectFolder)
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

#include <Objbase.h>
#include <Shobjidl.h>
#include <locale>
#include <codecvt>

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
	
	LPWSTR NAME;
	IShellItem *item;
	psiaResults->GetItemAt(0, &item);
	hr = item->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &NAME);
	psiaResults->Release();
	pfd->Release();

	//fixed later, convert UUID to humanreadable name

	std::wstring path(NAME);
	STDSTRING text;
	if (sizeof(TCHAR) == 1){
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		text = conv.to_bytes(path);
	}
	DUITRACE("SELECTED FOLDER : %s", text.c_str());
	
	//fixed 
	label->SetText(text.c_str());
}