#pragma once

#include <DuiLib/UIlib.h>

class CRecordPage
	:public CNotifyPump
{
public:
	CRecordPage();
	~CRecordPage();

	void SetPaintMagager(CPaintManagerUI* pPaintMgr);
	DUI_DECLARE_MESSAGE_MAP();
	void OnRecord(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);
private:
	CPaintManagerUI* ppm;
	time_t time;
	CLabelUI* timelabel;
};

