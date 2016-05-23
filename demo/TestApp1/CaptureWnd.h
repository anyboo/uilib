#pragma once
#include <DuiLib/UIlib.h>
class CCaptureWnd
	:public WindowImplBase
{
public:
	CCaptureWnd();
	~CCaptureWnd();
protected:
	virtual CDuiString GetSkinFolder() = 0;
	virtual CDuiString GetSkinFile() = 0;
	virtual LPCTSTR GetWindowClassName(void) const = 0;
private:
	CControlUI* m_pDesktopImage;
	CControlUI* m_pDesktopMask;
	CControlUI* m_pDesktopMaskBorder;
	CContainerUI* m_pDesktopCanvasContainer;

	RECT	Window;
	RECT	ClipPadding;
	POINT	ClipBasePoint; // adjust base point
	bool	bClipChoiced; //  already selected screenshot area
	CDuiString	ClipDrawStringBuf;
};

