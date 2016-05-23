#pragma once
#include <DuiLib/UIlib.h>

class CSettingWnd :
	public WindowImplBase
{
public:
	CSettingWnd();
	~CSettingWnd();
	
	virtual void InitWindow();
	virtual void OnFinalMessage(HWND /*hWnd*/);
	virtual void Notify(TNotifyUI& msg);
	DUI_DECLARE_MESSAGE_MAP();
	void OnSelectFolder(TNotifyUI& msg);
protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
private:
	CLabelUI* label;
};

