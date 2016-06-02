#include "stdafx.h"
#include "Setting.h"
#include "RecordHandler.h"
#include "CaptureWnd.h"
#include "Util.h"
#include "StaticPage.h"

CStaticPage::CStaticPage()
:ppm(nullptr), handler(CRecordHandler::Inst()), mute(false)
{
}

CStaticPage::~CStaticPage()
{
}

DUI_BEGIN_MESSAGE_MAP(CStaticPage, CNotifyPump)
	DUI_ON_CLICK_CTRNAME(CTR_RECORD, OnRecord)
	DUI_ON_CLICK_CTRNAME(CTR_SCREEN_CAPTURE, OnScreenCapture)
	DUI_ON_CLICK_CTRNAME(CTR_AREA_RECORD, OnAreaRecord)
	DUI_ON_CLICK_CTRNAME(CTR_LOCATION, OnLocation)
	DUI_ON_CLICK_CTRNAME(CTR_ENCODE, OnEncode)
	DUI_ON_CLICK_CTRNAME(CTR_VOICE, OnVoice)
DUI_END_MESSAGE_MAP()

void CStaticPage::SetPaintMagager(CPaintManagerUI* pPaintMgr)
{
	assert(pPaintMgr);
	ppm = pPaintMgr;
}

void CStaticPage::Switch()
{
	CTabLayoutUI* tab_main = dynamic_cast<CTabLayoutUI*>(ppm->FindControl(_T("tab_main")));

	if (!tab_main) return;

	CTabLayoutUI& t = *tab_main;

	t.SelectItem(1);

	ppm->SetTimer(t.FindSubControl(_T("record_page")), 0x5201, 1 * 1000);
}

void CStaticPage::OnRecord(TNotifyUI& msg)
{
	assert(ppm);

	RECT rc;
	::GetWindowRect(::GetDesktopWindow(),&rc);

	POINT P = { rc.left, rc.top};
	SIZE S = { rc.right - rc.left, rc.bottom - rc.top};

	handler.SetArea(P, S);
	handler.start();

	Switch();
}


void CStaticPage::OnScreenCapture(TNotifyUI& msg)
{
	trace(msg);
	//capture full screenshot
	HBITMAP bmp = CreateDesktopBitmap(0);
	HWND hWndDesktop = ::GetDesktopWindow();
	RECT rect;
	GetWindowRect(hWndDesktop, &rect);
	int width, height;
	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	CSetting& setting = CSetting::Inst();
	//test if work with chinese path
	std::wstring path;
	setting.GetLocation(path);

	BitmapToJpg(bmp, width, height, path);
}

void CStaticPage::OnAreaRecord(TNotifyUI& msg)
{
	trace(msg);
	CCaptureWnd Frame;

	Frame.Create(NULL, NULL, WS_VISIBLE | WS_POPUP, 0);
	Frame.CenterWindow();
	Frame.ShowModal();

	if (!Frame.IsClipChoiced()) return;

	RECT rc = Frame.GetClipRect();

	POINT P = {rc.left, rc.top};
	SIZE S = { rc.right - rc.left, rc.bottom - rc.top };

	handler.SetArea(P, S);
	handler.start();

	Switch();
}

#include "Shellapi.h"

void CStaticPage::OnLocation(TNotifyUI& msg)
{
	trace(msg);
	
	CSetting& setting = CSetting::Inst();
	//test if work with chinese path
	std::wstring path;
	setting.GetLocation(path);
	ShellExecute(NULL, L"explore", path.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

#include <map>
void CStaticPage::OnEncode(TNotifyUI& msg)
{
	trace(msg);

	typedef std::map<std::string,std::wstring> EncodeList;
	typedef EncodeList::iterator EncodeListIter;

	//初始化编码表
	//Initialize encode tabel
	//InitEncodeTabel()
	EncodeList encodes;
	encodes["MP4"] = L"record/codingMP4.png";
	encodes["FLV"] = L"record/codingFLV.png";

	//获取设置信息
	CSetting& setting = CSetting::Inst();
	std::string key = setting.GetEncode();

	//获取下一个编码表项，设置控件图片；如果是最后一项，则，选择第一个
	assert(ppm);
	CButtonUI* control = dynamic_cast<CButtonUI*>(ppm->FindControl(_T("btencode")));
	if (!control) return;
	
	/*
	EncodeListIter it = encodes.find(key);

	(it != encodes.end()) ? it : (it = encodes.begin());
	*/
	/*setting.SetEncode(it->first);
	control->SetNormalImage((it->second).c_str());*/
}

void CStaticPage::OnVoice(TNotifyUI& msg)
{
	trace(msg);

	mute = !mute;
	handler.SetMicro(mute);
	handler.SetVolume(mute);
}