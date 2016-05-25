#pragma once
#include <DuiLib/UIlib.h>

class CCaptureWnd
	:public WindowImplBase
{
public:
	CCaptureWnd();
	~CCaptureWnd();
protected:
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual void InitWindow();
	virtual void Notify(TNotifyUI& msg);
	DUI_DECLARE_MESSAGE_MAP()
protected:
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	CControlUI* Image;
	CControlUI* Mask;
	CControlUI* MaskBorder;
	CContainerUI* CanvasContainer;

	RECT	Window;
	RECT	ClipPadding;
	POINT	ClipBasePoint; // adjust base point
	bool	bClipChoiced; //  already selected screenshot area
	CDuiString	ClipDrawStringBuf;
	CSetting config;
};

