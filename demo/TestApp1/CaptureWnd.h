#pragma once
#include <DuiLib/UIlib.h>

class CCaptureWnd
	:public WindowImplBase
{
public:
	CCaptureWnd();
	~CCaptureWnd();

	void SetClipPadding(RECT rc);
	RECT GetClipPadding() const;
	RECT GetClipRect() const;
	RECT GetWindowRect() const;
	RECT GetCanvasContainerRect() const;
	bool IsClipChoiced() const;

	virtual void InitWindow();
	virtual void OnFinalMessage(HWND hWnd);
	virtual void Notify(TNotifyUI& msg);
protected:
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	DUI_DECLARE_MESSAGE_MAP()

	void OnLBUTTONDOWN(WPARAM wParam, LPARAM lParam);
	void OnLBUTTONUP(WPARAM wParam, LPARAM lParam);
	void OnLBUTTONDBLCLK(WPARAM wParam, LPARAM lParam);
	void OnRBUTTONUP(WPARAM wParam, LPARAM lParam);
	void OnMOUSEMOVE(WPARAM wParam, LPARAM lParam);
protected:
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/);
private:
	CControlUI* Image;
	CControlUI* Mask;
	CControlUI* MaskBorder;
	CContainerUI* CanvasContainer;

	RECT	Desktop;
	RECT	ClipPadding;
	POINT	ClipBasePoint; // adjust base point
	bool	bClipChoiced; //  already selected screenshot area
	CDuiString	ClipDrawStringBuf;
};

