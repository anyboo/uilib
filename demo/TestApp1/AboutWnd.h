#pragma once
#include <DuiLib/UIlib.h>

class CAboutWnd :
	public WindowImplBase
{
public:
	CAboutWnd();
	/*Destructor need to be a virtual member when the class is base*/
	~CAboutWnd();
	
	virtual void OnFinalMessage(HWND /*hWnd*/);
	virtual void Notify(TNotifyUI& msg);
	DUI_DECLARE_MESSAGE_MAP();

protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
};

