#pragma once
#include <DuiLib/UIlib.h>
#include <vector>

#define  BT_SearchLog		(_T("search"))

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

	void exportLog();
protected:
	vector<Log_Search>		m_LogInfo;
	vector<Case_Search>		m_CaseInfo;
protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
};
