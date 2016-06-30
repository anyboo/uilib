#pragma once
#include <DuiLib/UIlib.h>

#define BT_CLOSE			(_T("closebtn"))
#define	BT_MINWIND			(_T("minbtn"))
#define BT_DOWNLOAD			(_T("download"))
#define BT_LogWnd			(_T("Log_manager"))
#define BT_OtherTools		(_T("Other_tools"))
#define BT_VideoPlay		(_T("VideoPlay"))

class CMainWnd :
	public WindowImplBase
{
public:
	CMainWnd();
	~CMainWnd();

	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);

	virtual LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	DUI_DECLARE_MESSAGE_MAP();

	void OnMin(TNotifyUI& msg);
	void OnClose(TNotifyUI& msg);
	void OnDownLoadWnd(TNotifyUI& msg);
	void OnLogWnd(TNotifyUI& msg);
	void OnOtherToolsWnd(TNotifyUI& msg);

	void Show_HideTask(BOOL IsHide);

private:
	BOOL	m_IsMinWnd;

protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
};