
#pragma once
#include <DuiLib/UIlib.h>
#include <vector>



class CVideoVendorUI :
	public WindowImplBase
{
public:
	CVideoVendorUI();
	~CVideoVendorUI();
	virtual void OnFinalMessage(HWND /*hWnd*/);
	virtual void Notify(TNotifyUI& msg);
	DUI_DECLARE_MESSAGE_MAP();

	STDSTRING intToString(int num);
	void GetPrevPage();
	void GetNextPage();
	void SelectEquipment();

protected:
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
};
