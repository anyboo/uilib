#pragma once
#include <DuiLib/UIlib.h>

class CRecordHandler;
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
	void OnEncodeMP4(TNotifyUI& msg);
	void OnEncodeFLV(TNotifyUI& msg);
	void OnMute(TNotifyUI& msg);
	void OnVoice(TNotifyUI& msg);
protected:
	void Switch();
private:
	CPaintManagerUI* ppm;
	CRecordHandler& handler;
	bool mute;
};

