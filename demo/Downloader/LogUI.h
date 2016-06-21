#pragma once
#include <DuiLib/UIlib.h>
#include <vector>

#define  BT_SearchLog		(_T("search"))
#define		BT_OnStartTime		(_T("DataTime1"))
#define	 BT_OnStopTime		(_T("DataTime2"))

typedef struct 
{
	STDSTRING	Time;
	STDSTRING	Handle;
	STDSTRING	Description;
}Log_Search;

typedef struct 
{
	STDSTRING	Time;
	STDSTRING	Address;
	STDSTRING	Collect_people;
	STDSTRING	CaseName;
	STDSTRING	Case_description;
	STDSTRING	EquipmentIP;
	STDSTRING	Remark;
}Case_Search;

class CLogUI :
	public WindowImplBase
{
public:
	CLogUI();
	~CLogUI();
	virtual void OnFinalMessage(HWND /*hWnd*/);
	virtual void Notify(TNotifyUI& msg);
	DUI_DECLARE_MESSAGE_MAP();
	void OnBeginSearch(TNotifyUI& msg);

	void OnSearchLog();
	void OnSearchCaseLog();

	void CreateLogList();
	void CreateCaseList();

	void InsertLogInfoToList();
	void InsertCaseInfoToList();

	void OnSelectStartTime(TNotifyUI& msg);
	void OnSelectStopTime(TNotifyUI& msg);
	void exportLog();
protected:
	vector<Log_Search>		m_LogInfo;
	vector<Case_Search>		m_CaseInfo;
	CListUI*				m_pList;
	BOOL					m_bInit;
protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
};
