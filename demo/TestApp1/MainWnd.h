#pragma once
#include <DuiLib/UIlib.h>
#include "StaticPage.h"
#include "RecordPage.h"

class CMainWnd : 
	public WindowImplBase
{
public:
	CMainWnd();
	~CMainWnd();

	virtual void InitWindow();
	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);

	DUI_DECLARE_MESSAGE_MAP();

	void OnAbout(TNotifyUI& msg);
	void OnSettingDlg(TNotifyUI& msg);
	void OnMin(TNotifyUI& msg);
	void OnClose(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);
protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
private:
	CStaticPage Page1;
	CRecordPage Page2;
};

