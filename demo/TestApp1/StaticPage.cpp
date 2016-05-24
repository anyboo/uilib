#include "stdafx.h"
#include "StaticPage.h"

CStaticPage::CStaticPage()
:ppm(nullptr)
{
	//sscobs_init();
}

CStaticPage::~CStaticPage()
{
	//sscobs_uninit();
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


}

void CStaticPage::OnScreenCapture(TNotifyUI& msg)
{
	trace(msg);
	//capture full screenshot
	//CDuiString file = GetConfig().savefile;
	RECT rc;
	::GetWindowRect(::GetDesktopWindow(), &rc);
	//sscobs_startRecording(file,rc.x,rc.y,width,height);
}

void CStaticPage::OnAreaRecord(TNotifyUI& msg)
{
	trace(msg);
	CaptureWnd
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
}

void CStaticPage::OnVoice(TNotifyUI& msg)
{
	trace(msg);
	//enable or disable record voice
	//Popup Menu
}