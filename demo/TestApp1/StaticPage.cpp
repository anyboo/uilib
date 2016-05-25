#include "stdafx.h"
#include "StaticPage.h"
#include "RecordHandler.h"

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


void CStaticPage::OnRecord(TNotifyUI& msg)
{
	assert(ppm);
	CTabLayoutUI* tab_main = dynamic_cast<CTabLayoutUI*>(ppm->FindControl(_T("tab_main")));

	if (!tab_main) return;

	CTabLayoutUI& t = *tab_main;

	t.SelectItem(1);

	ppm->SetTimer(t.FindSubControl(_T("record_page")), 0x5201, 1*1000);

	RECT rc;
	::GetWindowRect(::GetDesktopWindow(),&rc);

	POINT P = { rc.left, rc.top};
	SIZE S = { rc.right - rc.left, rc.bottom - rc.top};

	handler.SetArea(P, S);
	handler.start();
}

void CStaticPage::OnScreenCapture(TNotifyUI& msg)
{
	trace(msg);
	//capture full screenshot
	//CDuiString file = GetConfig().savefile;
	/*RECT rc;
	::GetWindowRect(::GetDesktopWindow(), &rc);
	
	POINT P = { rc.left, rc.top };
	SIZE S = { rc.right - rc.left, rc.bottom - rc.top };

	handler.SetArea(P, S);
	handler.start();*/
}

void CStaticPage::OnAreaRecord(TNotifyUI& msg)
{
	trace(msg);
	//CaptureWnd
	/*RECT rc;

	::GetWindowRect(::GetDesktopWindow(), &rc);

	POINT P = { rc.left, rc.top };
	SIZE S = { rc.right - rc.left - 100, rc.bottom - rc.top - 100};

	handler.SetArea(P, S);

	handler.start();*/
}

void CStaticPage::OnLocation(TNotifyUI& msg)
{
	trace(msg);
	//open path of storage record video 
}

void CStaticPage::OnEncode(TNotifyUI& msg)
{
	trace(msg);
	//Popup Menu
	handler.SetEncode(ENCODE::MP4);
}

void CStaticPage::OnVoice(TNotifyUI& msg)
{
	trace(msg);
	//enable or disable record voice
	//Popup Menu
	mute = !mute;
	handler.SetMicro(mute);
	handler.SetVolume(mute);
}