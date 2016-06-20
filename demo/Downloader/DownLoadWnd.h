#pragma once
#include <DuiLib/UIlib.h>
#include <vector>
		
#define	BT_Calendar1			(_T("DataTime1"))
#define	BT_Calendar2			(_T("DataTime2"))
#define BT_OnVideoLoginUI		(_T("Add_device"))

typedef struct 
{
	STDSTRING	filename;
	int			filesize;
	STDSTRING	process;
	STDSTRING	speed;
	STDSTRING	remain_time;
	STDSTRING	state;
	STDSTRING	handle;
}FILE_INFO;

class DownLoadWnd :
	public WindowImplBase
{
public:
	DownLoadWnd();
	~DownLoadWnd();
	virtual void OnFinalMessage(HWND /*hWnd*/);
	virtual void Notify(TNotifyUI& msg);
	DUI_DECLARE_MESSAGE_MAP();
	void OnSelectTimeType();
	void OnSelectCalendar();

	void OnVideoLoginWnd(TNotifyUI& msg);

	void SearchFiles();

protected:
	vector<FILE_INFO>	m_fileInfo;
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
};

