#pragma once
#include <DuiLib/UIlib.h>


class CLogUI :
	public WindowImplBase
{
public:
	CLogUI();
	~CLogUI();
	virtual void OnFinalMessage(HWND /*hWnd*/);
	virtual void Notify(TNotifyUI& msg);
	DUI_DECLARE_MESSAGE_MAP();
	//void OnClose(TNotifyUI& msg);


protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
};
