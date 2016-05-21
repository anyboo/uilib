#pragma once
#include <DuiLib/UIlib.h>

class CStaticPage
	:public CNotifyPump
{
public:
	CStaticPage();
	~CStaticPage();

	void SetPaintMagager(CPaintManagerUI* pPaintMgr);
	DUI_DECLARE_MESSAGE_MAP();

	void OnRecord(TNotifyUI& msg);
	void OnScreenCapture(TNotifyUI& msg);
	void OnAreaRecord(TNotifyUI& msg);
	void OnLocation(TNotifyUI& msg);
	void OnEncode(TNotifyUI& msg);
	void OnVoice(TNotifyUI& msg);

private:
	CPaintManagerUI* ppm;
};

