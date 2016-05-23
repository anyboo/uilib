#include "stdafx.h"
#include "RecordPage.h"
#include <ctime>
#include <sstream>
#include <iomanip>

CRecordPage::CRecordPage()
:ppm(nullptr), time(0)
{
	
}

CRecordPage::~CRecordPage()
{
}

DUI_BEGIN_MESSAGE_MAP(CRecordPage, CNotifyPump)
	DUI_ON_CLICK_CTRNAME(CTR_RECORD, OnRecord)
	DUI_ON_TIMER()
DUI_END_MESSAGE_MAP()

void CRecordPage::SetPaintMagager(CPaintManagerUI* pPaintMgr)
{
	ppm = pPaintMgr;
}

void CRecordPage::OnRecord(TNotifyUI& msg)
{
	assert(ppm);
	CTabLayoutUI* tab_main = dynamic_cast<CTabLayoutUI*>(ppm->FindControl(_T("tab_main")));

	if (!tab_main) return;

	CTabLayoutUI& t = *tab_main;
	
	t.SelectItem(0);

	ppm->KillTimer(t.FindSubControl(_T("record_page")));
	time = 0;
	timelabel = dynamic_cast<CLabelUI*>(ppm->FindControl(_T("LbTime")));
	timelabel->SetText("00:00:00");
}

void CRecordPage::OnTimer(TNotifyUI& msg)
{
	trace(msg);

	time += 3;
	std::stringstream ss;
	auto tm = *std::gmtime(&time);
	ss << std::put_time(&tm, "%H:%M:%S") << std::endl;
	
	timelabel = dynamic_cast<CLabelUI*>(ppm->FindControl(_T("LbTime")));
	timelabel->SetText(ss.str().c_str());
}
