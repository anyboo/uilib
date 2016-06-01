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
	RECT rc1 = Frame.GetCanvasContainerRect();
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

void CStaticPage::OnEncode(TNotifyUI& msg)
{
	trace(msg);
	//Popup Menu
	//setting.SetEncode(ENCODE::MP4);
}

void CStaticPage::OnVoice(TNotifyUI& msg)
{
	trace(msg);

	mute = !mute;
	handler.SetMicro(mute);
	handler.SetVolume(mute);
}