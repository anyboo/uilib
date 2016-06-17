#pragma once
#include <DuiLib/UIlib.h>

#define BT_CLOSE			(_T("closebtn"))
#define	BT_MINWIND			(_T("minbtn"))
#define BT_DOWNLOAD			(_T("download"))
#define BT_LogWnd			(_T("Log_manager"))
#define BT_VideoPlay		(_T("VideoPlay"))
#define BT_VideoLogin		(_T("Other_tools"))

class CMainWnd :
	public WindowImplBase
{
public:
	CMainWnd();
	~CMainWnd();

	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);

	DUI_DECLARE_MESSAGE_MAP();

	void OnMin(TNotifyUI& msg);
	void OnClose(TNotifyUI& msg);
	void OnDownLoadWnd(TNotifyUI& msg);
	void OnLogWnd(TNotifyUI& msg);
	void OnVideoLonginWnd(TNotifyUI& msg);

protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
};